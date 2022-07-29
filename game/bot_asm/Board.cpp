#include "Board.h"

Board::Board(uint16_t max_size) : max_size(max_size), memsize(0) {}

uint16_t Board::get_mem_size() { return memsize; }

void Board::initialise_botberri() {
    // add all botmodules to the bus

    vis_ip = std::make_shared<VisionInput>(IO_VISION_START, IO_VISION_SIZE);
    dec_op = std::make_shared<DecisionOutput>(IO_DECISION_START, IO_DECISION_SIZE);

    fsm_mem = std::make_shared<Memory>(MEM_FSM_START, MEM_FSM_SIZE);
    instr_mem = std::make_shared<Memory>(MEM_START, memsize);
    stack_mem = std::make_shared<Memory>(MEM_START + memsize, max_size - memsize);

    uint16_t i = 0;
    ByteSink sink = [this, &i](uint8_t byte) { instr_mem->writeOneByte(byte, MEM_START + i++); };

    for (CPUInstruction::Instruction in : instructions) {
        in.encode(sink);
    }

    bus = std::make_unique<Bus>();

    bus->addBotModule(instr_mem);
    bus->addBotModule(stack_mem);
    bus->addBotModule(fsm_mem);
    bus->addBotModule(vis_ip);
    bus->addBotModule(dec_op);
    cpu = std::make_unique<CPU>(MEM_START, MEM_START + memsize, *bus);
}
void Board::load_from_file(std::string& filename) {
    instructions = assembler.assemble_file(filename, MEM_START);
    memsize = assembler.getMemSize();
}

void Board::cpu_step() {
    if (cpu == nullptr) {
        throw BoardError("board not initialized!");
    }

    cpu->step();
}

void Board::add_instruction(CPUInstruction::Instruction ins) {
    instructions.push_back(ins);
    memsize += ins.size();
}

void Board::load_and_start_code(std::string& filename) {
    load_from_file(filename);
    initialise_botberri();
}

void Board::set_vision_bitmask(uint32_t mask) { vis_ip->setVisionBitMask(mask); }

uint8_t Board::get_next_move() { return dec_op->popDecision(); }

bool Board::isDecisionAvailable() { return dec_op->isDecisionAvailable(); }

void Board::loadFSM(uint8_t* fsm_tabel) {
    for (size_t i = 0; i < 1024; i++) {
        bus->writeOneByte(fsm_tabel[i], MEM_FSM_START + i);
    }
}
