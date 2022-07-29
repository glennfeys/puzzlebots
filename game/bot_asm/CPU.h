#ifndef SYSPROG_BOTCPU_H
#define SYSPROG_BOTCPU_H

#include "BitHelper.h"
#include "Bus.h"
#include "CPUInstruction.h"
#include <assert.h>
#include <cstdint>
#include <stack>
#include <utility>

class CPU {
  private:
    Bus bus;
    bot_register_t registers[8];
    CPUInstruction::Instruction currentInstruction =
        CPUInstruction::Instruction(CPUInstruction::Operation::NOP);
    CPUInstruction::Operation currentOperation;

  public:
    //constructor for cpu with first Inststruction address and stack address
    CPU(uint16_t, uint16_t, Bus&);
    //performs the next operation in the cpu
    void step();
    //gives the registers
    uint16_t* getRegisters();
    //Processes the current Operation
    uint8_t processOperation();
    // all operations that the CPU needs to be able to do 
    void do_NOP();
    void do_MOV();
    void do_MOVB();
    void do_PUSH();
    void do_POP();
    void do_ADD();
    void do_SUB();
    void do_AND();
    void do_OR();
    void do_XOR();
    void do_NOT();
    void do_SHR();
    void do_SHL();
    void do_INC();
    void do_DEC();
    void do_CMP();
    void do_TEST();
    void do_JMP();
    void do_JE();
    void do_JNE();
    void do_CALL();
    void do_RET();
};

#endif // SYSPROG_BOTCPU_H
