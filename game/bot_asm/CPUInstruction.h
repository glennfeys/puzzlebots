#ifndef SYSPROG_CPUINSTRUCTION_H
#define SYSPROG_CPUINSTRUCTION_H

#include "BitHelper.h"
#include "BotBerryConstants.h"
#include "Bus.h"

#include <assert.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>

// register 0 -> %pc == $ip  (progam counter aka instruction pointer)
// register 2 -> %bp         (base pointer)
// register 1 -> %sp         (stack pointer)
#define REG_IP 0
#define REG_SP 1
#define REG_BP 2
#define REG_A 3
#define REG_B 4
#define REG_C 5
#define REG_D 6
#define REG_E 7

namespace CPUInstruction {
typedef enum CPUInstructionOperandType {
    IMMEDIATE = 0,
    REGISTER = 1,
    MEM_IMMEDIATE = 2,
    MEM_REGISTER = 3,
    NO_ARG = 4
} CPUInstructionOperandType;

typedef enum Operation {
    NOP = 0,
    MOV = 1,
    MOVB = 2,
    PUSH = 3,
    POP = 4,
    ADD = 5,
    SUB = 6,
    AND = 7,
    OR = 8,
    XOR = 9,
    NOT = 10,
    SHR = 11,
    SHL = 12,
    INC = 13,
    DEC = 14,
    CMP = 15,
    TEST = 16,
    JMP = 17,
    JE = 18,
    JNE = 19,
    CALL = 20,
    RET = 21
} CPUInstructionType;

// aantal argumenten per operation
extern uint8_t operationArguments[];

/**
 * The instruction class can be useful as an intermediate format between:
 *   - assemble code (text) and binary CPU instructions
 *   - binary CPU instructions and the execution of them on the CPU
 *
 * You are not required to use this class.
 */
class Argument {
  private:
    // operandType
    uint8_t type;
    // given value
    uint16_t value;
    // displacement when needed
    uint8_t displacement;

  public:
    // constructor for empty argument
    Argument();
    // constuructor for argument with given type and value
    Argument(uint8_t type, uint16_t value);
    uint8_t getType() const;
    uint16_t getValue() const;
    void setDisplacement(uint8_t);
    uint8_t getDisplacement() const;
    // reads from argument
    uint16_t read(uint16_t[], uint8_t, Bus);
    // writes to argument
    void write(uint16_t, uint16_t[], uint8_t, Bus);
};

class Instruction {
  public:
    // Constructor for empty Instruction
    Instruction() = default;
    // Constructor for construction for given operation
    Instruction(CPUInstruction::Operation operation);
    // Adds argument to arguments
    void addArgument(Argument);
    Argument getArgument(int pos) const;
    Argument* getArguments();
    CPUInstruction::Operation getOperation() const;
    // amount of bytes in the Instruction
    uint16_t size();
    /**
     * Encode this instruction, by writing it's bytes to the provided byteSink.
     *
     * @param byteSink a lambda used to write the bytes
     * @return the number of bytes written (= number of bytes of the written instruction)
     */
    uint8_t encode(std::function<void(uint8_t)> byteSink) const;
    /**
     * Decode the instruction read from the byteSource.
     * This throws an exception when an invalid instruction is read.
     *
     * @param byteSource a lambda used to read bytes
     * @param bytes_read (return value) returns the number of bytes read from the byteSource (=
     * number of bytes of the read instruction)
     * @return the read CPU Instruction
     */
    static Instruction decode(std::function<uint8_t()> byteSource, uint8_t& bytes_read);

  private:
    Operation operation;
    // amount of set arguments
    uint8_t argPos = 0;
    Argument arguments[2];
};

bool operator==(const Instruction& me, const Instruction& other);
bool operator==(const Argument& me, const Argument& other);

} // namespace CPUInstruction

#endif // SYSPROG_CPUINSTRUCTION_H
