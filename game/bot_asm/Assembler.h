#ifndef SYSPROG_ASSEMBLER_H
#define SYSPROG_ASSEMBLER_H

#include <algorithm>
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "BotBerryConstants.h"
#include "CPUInstruction.h"
using CPUInstruction::CPUInstructionOperandType;

class AssemblerError : public std::runtime_error {
  public:
    explicit AssemblerError(const std::string& msg);
    explicit AssemblerError(const char* msg);
};

class AssemblerErrorWithLine : public std::runtime_error {
  private:
    const int line_nr;

  public:
    AssemblerErrorWithLine(int line_nr, const std::string& msg);
    AssemblerErrorWithLine(int line_nr, const char* msg);
};

typedef enum TokenType {
    EMPTY,     // empty space...
    LABEL,     // LabelName:
    DATASPEC,  // .(byte|word) data
    COMMENT,   //   //  /*   #
    DATA_NR,   //  [0-9]* or 0x[0-9]*
    OPERATION, // ex: MOV, ADD, NOP, JMP, etc.
    OPERAND,   // ex: %a, -6(%sp), $IMMEDIATE, ($MEM_ADDRESS), $42, ($0xFF00)
    EQU,       //.equ
    EQU_ID,    // id after equ, ex:  MEM_ADDRESS  in ".equ MEM_ADDRESS 0xFF00"
    PARSE_ERROR_DOTUNKNOWN,
    PARSE_ERROR_OTHER
} TokenType;

class AssemblerToken {
  public:
    const TokenType tokenType;
    const std::string val; // value, stripped of syntax "decorators", like : . " 0x and ,
    AssemblerToken(const TokenType& tokenType, const std::string& val);
    bool operator==(const AssemblerToken& other);
    friend std::ostream& operator<<(std::ostream& os, const AssemblerToken& t);
};

class Assembler {
  private:
    const std::unordered_map<std::string, CPUInstruction::Operation> op_map;

    const std::regex label_regex;
    const std::regex dataspec_regex;
    const std::regex comment_regex;
    const std::regex data_nr_regex;
    const std::regex operation_regex;
    const std::regex operand_regex;
    const std::regex equ_regex;
    const std::regex equid_regex;

    const std::regex operand_reg_regex;
    const std::regex operand_imm_regex;
    const std::regex operand_address_regex;
    const std::regex operand_mem_reg_regex;

  public:
    static const std::unordered_map<std::string, uint16_t> regBits;
    Assembler(/*aanvullen indien nodig*/);

    // aan te vullen

    // enkele handige functies zijn reeds ter beschikking:
    // tip: bij het parsen van de assembler code, splits elke lijn eerst in tokens met "tokenize" en
    //     verwerk vervolgens de tokens. Deze tussenstap maakt het parsen veel overzichtelijker.
    std::vector<AssemblerToken> tokenize(const std::string& line);
    CPUInstruction::Operation to_operation(const std::string& op_str);
    uint8_t reg_name_to_index(const std::string& reg_name);
    std::vector<CPUInstruction::Instruction> to_instructions(const std::string& line);
    uint16_t getLineSize();
    std::vector<CPUInstruction::Instruction> assemble_file(const std::string& path,
                                                           uint16_t mem_start);
    uint16_t getMemSize();

  private:
    bool labeling;
    uint16_t memsize;
    uint16_t mempos;
    std::unordered_map<std::string, uint16_t> labels;
    std::unordered_map<std::string, uint16_t> equs;
    CPUInstruction::Argument token_to_arg(const AssemblerToken& arg_token);
    CPUInstruction::Argument immval_to_arg(const std::string& val);
    CPUInstruction::Argument immaddr_to_arg(const std::string& addr);
    CPUInstruction::Argument regaddr_to_arg(const std::string& val);
    void make_label(std::vector<AssemblerToken> tokens);
    void make_equ(std::vector<AssemblerToken> tokens);
    void labelpass(std::ifstream& file);
    std::vector<CPUInstruction::Instruction> parsing_pass(std::ifstream& file);
    std::unique_ptr<std::pair<CPUInstruction::CPUInstructionOperandType, uint16_t>>
    rh_lookup(const std::string& val);
    uint16_t parse_val(const std::string& val);
    void preload();
};

/**
 * hulpfunctie om een string in stukken te splitsen.
 * Voorbeeld:
 *   std::string line(" aa bb  cc ");
 *   std::vector<std::string> parts = split(line, " ");
 *   BOOST_REQUIRE( parts.size() == 3 );
 *   BOOST_REQUIRE( parts[0] == "aa" );
 *   BOOST_REQUIRE( parts[1] == "bb" );
 *   BOOST_REQUIRE( parts[2] == "cc" );
 *
 * @param in de input string.
 * @param delim de "delimiter", dit is het teken waarom gesplits moet worden, bevoorbeeld een
 * spatie. (kan ook uit meerdere tekens bestaan, in dat geval moet de gehele "delim" overeenkomen.)
 * @return een vector met de delen van de opgesplitste string
 */
std::vector<std::string> split(const std::string& in, const std::string& delim);
template <size_t N1, size_t N2>
std::bitset<N1 + N2> build_bit_set(std::bitset<N1> bs1, std::bitset<N2> bs2) {
    std::bitset<N1 + N2> rslt;
    size_t i = 0;

    while (i < N2) {
        rslt[i] = bs2[i];
        i++;
    }
    size_t j = 0;

    while (j < N1) {
        rslt[i] = bs1[j];
        i++;
        j++;
    }
    return rslt;
}
#endif // SYSPROG_ASSEMBLER_H
