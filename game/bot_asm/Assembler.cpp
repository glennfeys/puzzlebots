#include "Assembler.h"

#include <assert.h>
#include <fstream>

using namespace CPUInstruction;

AssemblerError::AssemblerError(const std::string& msg) : runtime_error(msg) {}
AssemblerError::AssemblerError(const char* msg) : runtime_error(msg) {}

AssemblerErrorWithLine::AssemblerErrorWithLine(int line_nr, const std::string& msg)
    : runtime_error(msg + std::string(" @ line ") + std::to_string(line_nr)), line_nr(line_nr) {}
AssemblerErrorWithLine::AssemblerErrorWithLine(int line_nr, const char* msg)
    : runtime_error(std::string(msg) + std::string(" @ line ") + std::to_string(line_nr)),
      line_nr(line_nr) {}

static_assert(std::is_nothrow_copy_constructible<AssemblerError>::value,
              "AssemblerError must be nothrow copy constructible");
static_assert(std::is_nothrow_copy_constructible<AssemblerErrorWithLine>::value,
              "AssemblerErrorWithLine must be nothrow copy constructible");

std::vector<std::string> split(const std::string& in, const std::string& delim) {
    std::vector<std::string> res;
    uint8_t i = 0;
    int l = 0;
    int dlen = delim.length();
    while (in.substr(i, dlen) == delim && i < in.length()) {
        i += dlen;
    }
    while (i < in.length()) {
        if (in.substr(i, dlen) == delim) {
            i += dlen;
        } else {
            l = 0;
            while (in.substr(i, dlen) != delim && i < in.length()) {
                i++;
                l++;
            }
            res.push_back(in.substr(i - l, l));
        }
    }
    return res;
}

AssemblerToken::AssemblerToken(const TokenType& tokenType, const std::string& val)
    : tokenType(tokenType), val(val) {}

bool AssemblerToken::operator==(const AssemblerToken& other) {
    return this->tokenType == other.tokenType && this->val == other.val;
}

std::ostream& operator<<(std::ostream& os, const AssemblerToken& t) {
    std::string typestr;
    switch (t.tokenType) {
    case EMPTY:
        os << "EMPTY";
        return os;
    case LABEL:
        typestr = "LABEL";
        break;
    case DATASPEC:
        typestr = "DATASPEC";
        break;
    case COMMENT:
        typestr = "COMMENT";
        break;
    case DATA_NR:
        typestr = "DATA_NR";
        break;
    case OPERATION:
        typestr = "OPERATION";
        break;
    case OPERAND:
        typestr = "OPERAND";
        break;
    case EQU:
        typestr = "EQU";
        break;
    case EQU_ID:
        typestr = "EQU_ID";
        break;
    case PARSE_ERROR_DOTUNKNOWN:
        typestr = "PARSE_ERROR_DOTUNKNOWN";
        break;
    case PARSE_ERROR_OTHER:
        typestr = "PARSE_ERROR_OTHER";
        break;
    default:
        typestr = "Unknown token";
    }
    os << typestr << t.val;
    return os;
}

static std::unordered_map<std::string, CPUInstruction::Operation> init_op_map() {
    std::unordered_map<std::string, CPUInstruction::Operation> op_map;
    op_map["NOP"] = NOP;
    op_map["MOV"] = MOV;
    op_map["MOVB"] = MOVB;
    op_map["PUSH"] = PUSH;
    op_map["POP"] = POP;
    op_map["ADD"] = ADD;
    op_map["SUB"] = SUB;
    op_map["AND"] = AND;
    op_map["OR"] = OR;
    op_map["XOR"] = XOR;
    op_map["NOT"] = NOT;
    op_map["SHR"] = SHR;
    op_map["SHL"] = SHL;
    op_map["INC"] = INC;
    op_map["DEC"] = DEC;
    op_map["CMP"] = CMP;
    op_map["TEST"] = TEST;
    op_map["JMP"] = JMP;
    op_map["JE"] = JE;
    op_map["JZ"] = JE; // JZ and JE do exactly the same
    op_map["JNE"] = JNE;
    op_map["JNZ"] = JNE; // JNZ and JNE do exactly the same
    op_map["CALL"] = CALL;
    op_map["RET"] = RET;
    return op_map;
}

Assembler::Assembler()
    : op_map(init_op_map()), label_regex("^([A-Za-z_-]+):$", std::regex_constants::ECMAScript),
      dataspec_regex("^\\.(byte|word)$", std::regex_constants::ECMAScript),
      comment_regex("^(//|#).*", std::regex_constants::ECMAScript),
      data_nr_regex("^([+-]?[0-9]+|0x[0-9a-fA-F]+),?$", std::regex_constants::ECMAScript),
      operation_regex("^([A-Za-z]+)$", std::regex_constants::ECMAScript),
      operand_regex("^([A-Za-z%$)_(0-9-]+),?$", std::regex_constants::ECMAScript),
      equ_regex("^\\.(equ)$", std::regex_constants::ECMAScript),
      equid_regex("^([a-zA-Z][a-zA-Z0-9_]*)$", std::regex_constants::ECMAScript),
      operand_reg_regex("^%([a-z]+|r[0-9]+)$", std::regex_constants::ECMAScript),
      operand_imm_regex("^\\$([A-Za-z_0-9-]+)$", std::regex_constants::ECMAScript),
      operand_address_regex("^\\(\\$([A-Za-z_0-9-]+)\\)$", std::regex_constants::ECMAScript),
      operand_mem_reg_regex("^([-+]?[0-9]+)?\\(%([a-z]+|r[0-9]+)\\)$",
                            std::regex_constants::ECMAScript),
      labeling(false), memsize(0), mempos(0), equs{{"MEM_FSM_START", MEM_FSM_START},
                                                   {"MEM_START", MEM_START},
                                                   {"IO_VISION_START", IO_VISION_START},
                                                   {"IO_DECISION_START", IO_DECISION_START}} {}

const std::unordered_map<std::string, uint16_t> Assembler::regBits{
    {"%ip", 0}, {"%sp", 1}, {"%bp", 2}, {"%a", 3}, {"%b", 4}, {"%c", 5}, {"%d", 6}, {"%e", 7},
};

std::vector<AssemblerToken> Assembler::tokenize(const std::string& line) {
    std::vector<std::string> words = split(line, " ");
    std::vector<AssemblerToken> res;
    TokenType prev_tokenType = EMPTY;
    for (auto it = words.begin(); it != words.end(); it++) {
        const std::string& word = *it;
        if (word.empty())
            continue;

        std::smatch sm;
        // first of all, whatever was previously seen, search for a new comment start
        if (regex_search(word, sm, comment_regex)) {
            prev_tokenType = COMMENT;
            continue;
        }

        switch (prev_tokenType) {
        case PARSE_ERROR_OTHER:
        case LABEL: {
            prev_tokenType = PARSE_ERROR_OTHER;
            res.push_back(AssemblerToken(PARSE_ERROR_OTHER, word));
            continue;
        }
        case COMMENT:
            continue;
        case EMPTY: {
            if (regex_search(word, sm, comment_regex)) {
                prev_tokenType = COMMENT;
                continue;
            }
            if (regex_search(word, sm, label_regex)) {
                prev_tokenType = LABEL;
                res.emplace_back(LABEL, sm[1]);
                continue;
            }
            if (regex_search(word, sm, dataspec_regex)) {
                res.emplace_back(DATASPEC, sm[1]);
                prev_tokenType = DATASPEC;
                continue;
            }
            if (regex_search(word, sm, equ_regex)) {
                res.emplace_back(EQU, sm[1]);
                prev_tokenType = EQU;
                continue;
            }
            if (regex_search(word, sm, operation_regex)) {
                bool is_op;
                try {
                    to_operation(word);
                    res.emplace_back(OPERATION, sm[1]);
                    prev_tokenType = OPERATION;
                    continue;
                } catch (AssemblerError& err) {
                    // This was not a valid operation
                    // Do nothing -> PARSE_ERROR_OTHER below
                }
            }
            if (!word.empty() && word[0] == '.') {
                prev_tokenType = PARSE_ERROR_DOTUNKNOWN;
                res.emplace_back(PARSE_ERROR_DOTUNKNOWN, word);
                continue;
            }
            prev_tokenType = PARSE_ERROR_OTHER;
            res.emplace_back(PARSE_ERROR_OTHER, word);
            continue;
        }
        case DATASPEC: {
            if (regex_search(word, sm, data_nr_regex)) {
                prev_tokenType = DATASPEC;
                res.emplace_back(DATA_NR, sm[1]);
                continue;
            }
            prev_tokenType = PARSE_ERROR_OTHER;
            res.emplace_back(PARSE_ERROR_OTHER, word);
            continue;
        }
        case OPERAND:
        case OPERATION: {
            if (regex_search(word, sm, operand_regex)) {
                prev_tokenType = OPERAND;
                res.emplace_back(OPERAND, sm[1]);
                continue;
            }
            prev_tokenType = PARSE_ERROR_OTHER;
            res.emplace_back(PARSE_ERROR_OTHER, word);
            continue;
        }
        case EQU: {
            if (regex_search(word, sm, equid_regex)) {
                prev_tokenType = EQU_ID;
                res.emplace_back(EQU_ID, sm[1]);
                continue;
            }
            prev_tokenType = PARSE_ERROR_OTHER;
            res.emplace_back(PARSE_ERROR_OTHER, word);
            continue;
        }
        case EQU_ID: {
            if (regex_search(word, sm, data_nr_regex)) {
                prev_tokenType = DATA_NR;
                res.emplace_back(DATA_NR, sm[1]);
                continue;
            }
            prev_tokenType = PARSE_ERROR_OTHER;
            res.emplace_back(PARSE_ERROR_OTHER, word);
            continue;
        }
        default: {
            prev_tokenType = PARSE_ERROR_OTHER;
            res.emplace_back(PARSE_ERROR_OTHER, word);
            continue;
        }
        }
    }
    return res;
}

Operation Assembler::to_operation(const std::string& op_str) {
    std::string base_op(op_str);
    auto to_upper = [](char ch) { return toupper(ch); };
    std::transform(base_op.begin(), base_op.end(), base_op.begin(), to_upper);
    std::unordered_map<std::string, Operation>::const_iterator it = op_map.find(base_op);
    if (it != op_map.end()) {
        return it->second;
    }
    throw AssemblerError(std::string("Error parsing OP '") + op_str + std::string("') "));
}

uint8_t Assembler::reg_name_to_index(const std::string& reg_name) {
    assert(reg_name != "r");
    if (reg_name.empty())
        throw AssemblerError("reg name empty");
    if (reg_name == "pc" || reg_name == "ip")
        return 0;
    if (reg_name == "sp")
        return 1;
    if (reg_name == "bp")
        return 2;
    if (reg_name == "a")
        return 3;
    if (reg_name == "b")
        return 4;
    if (reg_name == "c")
        return 5;
    if (reg_name == "d")
        return 6;
    if (reg_name == "e")
        return 7;
    throw AssemblerError(std::string("reg name invalid '") + reg_name + std::string("'"));
}

std::unique_ptr<std::pair<CPUInstructionOperandType, uint16_t>>
Assembler::rh_lookup(const std::string& val) {
    // this doesnt seem logical anymore, but basically the first value of the tuple is used to
    // distinguish a label from an equ which can then be handled accordingly by the caller. the fact
    // that im using the CPUInstructionOperandType enum for this is no longer relevant but too much
    // of a hassle to change at this stage
    std::unordered_map<std::string, uint16_t>::iterator it1 = labels.find(val);
    auto rslt = std::make_unique<std::pair<CPUInstructionOperandType, uint16_t>>();
    if (it1 != labels.end()) {
        rslt->first = MEM_IMMEDIATE;
        rslt->second = it1->second;
        return rslt;
    }
    std::unordered_map<std::string, uint16_t>::iterator it2 = equs.find(val);
    if (it2 != labels.end()) {
        rslt->first = IMMEDIATE;
        rslt->second = it2->second;
        return rslt;
    }
    return nullptr;
}

Argument Assembler::immval_to_arg(const std::string& val) {
    if (labeling) {
        return Argument(IMMEDIATE, 0);
    }
    std::string sub(val.substr(1));

    auto look = rh_lookup(sub);
    if (look != nullptr) {
        if (look->first == IMMEDIATE) {
            return Argument(IMMEDIATE, look->second);
        } else if (look->first == MEM_IMMEDIATE) {
            return Argument(IMMEDIATE, look->second);
        } else {
            throw AssemblerError("lookup error");
        }
    }
    unsigned long num = parse_val(sub);
    return Argument(IMMEDIATE, static_cast<uint16_t>(num));
}
Argument Assembler::immaddr_to_arg(const std::string& addr) {
    std::string sub(addr.substr(2, addr.length() - 3));
    if (labeling) {
        return Argument(MEM_IMMEDIATE, 0);
    }
    auto look = rh_lookup(sub);
    if (look != nullptr) {
        if (look->first == IMMEDIATE) {
            return Argument(MEM_IMMEDIATE, look->second);
        } else {
            throw AssemblerError("illegal use of label");
        }
    }
    return Argument(MEM_IMMEDIATE, static_cast<uint16_t>(parse_val(sub)));
}

Argument Assembler::regaddr_to_arg(const std::string& val) {
    size_t i = 0;

    while (val[i] != '(') {
        i++;
    }

    std::string addr(val.substr(i + 1, val.length() - 2 - i));
    Argument rslt(MEM_REGISTER, Assembler::regBits.at(addr));
    if (i != 0) {
        rslt.setDisplacement(static_cast<uint8_t>(parse_val(val.substr(0, i))));
    }
    return rslt;
}

Argument Assembler::token_to_arg(const AssemblerToken& arg_token) {
    std::smatch sm;
    if (regex_search(arg_token.val, sm, operand_imm_regex)) {
        return immval_to_arg(arg_token.val);
    }
    if (regex_search(arg_token.val, sm, operand_reg_regex)) {
        return Argument(REGISTER, regBits.at(arg_token.val));
    }

    if (regex_search(arg_token.val, sm, operand_address_regex)) {
        return immaddr_to_arg(arg_token.val);
    }
    if (regex_search(arg_token.val, sm, operand_mem_reg_regex)) {
        return regaddr_to_arg(arg_token.val);
    }
    throw AssemblerError(std::string("cant parse argument"));
}

void Assembler::make_label(std::vector<AssemblerToken> tokens) {

    labels.insert(std::pair<std::string, uint16_t>(tokens[0].val, mempos));
}
uint16_t Assembler::parse_val(const std::string& val) {
    // only give stripped values to this, aka "Ox10" or "5", not $0x10 or (5)
    int base = 10;
    if (val.substr(0, 2) == "0x" || val.substr(0, 2) == "0X") {
        base = 16;
    }
    size_t* pos = nullptr;
    return static_cast<uint16_t>(std::stoul(val, pos, base));
}
void Assembler::make_equ(std::vector<AssemblerToken> tokens) {
    if (tokens.size() < 3) {
        throw AssemblerError("syntax error");
    }
    std::string val(tokens[2].val);
    equs.insert(std::pair<std::string, uint16_t>(tokens[1].val, parse_val(val)));
}

std::vector<Instruction> Assembler::to_instructions(const std::string& line) {
    std::vector<AssemblerToken> tokens = tokenize(line);
    std::vector<Instruction> rslt;

    if (tokens.empty()) {
        return rslt;
    }

    if (tokens[0].tokenType == OPERATION) {
        Operation op = to_operation(tokens[0].val);
        if (tokens.size() - 1 != operationArguments[op]) {
            throw AssemblerError(std::string(""));
        }
        Instruction ins(op);

        for (size_t i = 1; i < tokens.size(); i++) {
            ins.addArgument(token_to_arg(tokens[i]));
        }
        rslt.push_back(ins);

    } else if (tokens[0].tokenType == LABEL) {
        if (labeling) {
            make_label(tokens);
        }
    } else if (tokens[0].tokenType == EQU) {
        if (labeling) {
            make_equ(tokens);
        }
    }
    return rslt;
}
void Assembler::preload() {
    std::ifstream botfile("game/bot_asm/programs/assem_header.botasm");
    mempos = MEM_START;
    labelpass(botfile);
}

std::vector<Instruction> Assembler::assemble_file(const std::string& path, uint16_t mem_start) {
    preload();
    std::ifstream botfile(path);
    mempos = mem_start;
    labelpass(botfile);
    botfile.clear();
    botfile.seekg(0, std::ios_base::beg);
    std::vector rslt = parsing_pass(botfile);

    botfile.close();
    return rslt;
}

void Assembler::labelpass(std::ifstream& file) {
    std::string line;
    labeling = true;
    while (std::getline(file, line)) {
        std::vector<CPUInstruction::Instruction> instructions = to_instructions(line);
        for (auto ins : instructions) {
            uint16_t size = ins.size();
            memsize += size;
            mempos += size;
        }
    }
}
std::vector<Instruction> Assembler::parsing_pass(std::ifstream& file) {
    std::vector<Instruction> rslt;
    std::string line;
    labeling = false;
    while (std::getline(file, line)) {
        std::vector<CPUInstruction::Instruction> instructions = to_instructions(line);
        for (auto ins : instructions) {
            rslt.push_back(ins);
        }
    }
    return rslt;
}

uint16_t Assembler::getLineSize() { return 0; }
uint16_t Assembler::getMemSize() { return memsize; }
