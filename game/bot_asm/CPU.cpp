#include "CPU.h"
#include <functional>

using namespace CPUInstruction;

//constructor fo the CPU
CPU::CPU(uint16_t ip, uint16_t sp, Bus& buss): bus(buss), registers{ip, sp}, currentInstruction(NOP) {}
uint16_t* CPU::getRegisters() { return registers; }

//does noothing
void CPU::do_NOP() {}
//moves 2 bytes of argument 1 to 2nd argument
void CPU::do_MOV() {
    uint16_t toWrite = currentInstruction.getArgument(0).read(registers, 2, bus);
    currentInstruction.getArgument(1).write(toWrite, registers, 2, bus);
    registers[2] = 0;
}
//moves 1 byte of argument 1 to 2nd argument
void CPU::do_MOVB() {
    uint8_t toWrite = currentInstruction.getArgument(0).read(registers, 1, bus);
    currentInstruction.getArgument(1).write(toWrite, registers, 1, bus);
    registers[2] = 0;
}
//pushes new items into the stack
void CPU::do_PUSH() {
    registers[1] -= 2;
    uint16_t toWrite = currentInstruction.getArgument(0).read(registers, 2, bus);
    bus.writeTwoBytes(toWrite, registers[1]);
}
//gets the first item from the stack and writes it to the second argument
void CPU::do_POP() {
    uint16_t popValue = bus.readTwoBytes(registers[1]);
    currentInstruction.getArgument(0).write(popValue, registers, 2, bus);
    registers[1] += 2;
    registers[2] = popValue;
}
//adds the first and second argument together and writes it to the second argument
void CPU::do_ADD() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = first + second;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//substracts the second argument from the first and writes it to the second argument
void CPU::do_SUB() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = first - second;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//does logical and operation on first and second argument and writes result to second argument
void CPU::do_AND() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = first & second;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//does logical or operation on first and second argument and writes result to second argument
void CPU::do_OR() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = first | second;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//does logical xor operation on first and second argument and writes result to second argument
void CPU::do_XOR() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = first ^ second;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//does logical not operation on first argument and writes result to this argument
void CPU::do_NOT() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t result = not first;
    currentInstruction.getArgument(0).write(result, registers, 2, bus);
    registers[2] = 0;
}
//does bitwise right shift operation on first and second argument and writes result to second argument
void CPU::do_SHR() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = second >> first;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//does bitwise left shift operation on first and second argument and writes result to second argument
void CPU::do_SHL() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    uint16_t result = second << first;
    currentInstruction.getArgument(1).write(result, registers, 2, bus);
    registers[2] = 0;
}
//Increments argument and writes it to the argument
void CPU::do_INC() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    first++;
    currentInstruction.getArgument(0).write(first, registers, 2, bus);
    registers[2] = 0;
}
//Decrements argument and writes it to the argument
void CPU::do_DEC() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    first--;
    currentInstruction.getArgument(0).write(first, registers, 2, bus);
    registers[2] = first;
}
//Compares if the 2 arguments whave the same value and sets the result as the flag
void CPU::do_CMP() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    registers[2] = second - first;
}
//does and operation on both arguments and sets the result as the flag
void CPU::do_TEST() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    uint16_t second = currentInstruction.getArgument(1).read(registers, 2, bus);
    registers[2] = second & first;
}
//jumps to the instruction on the given argument
void CPU::do_JMP() {
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    registers[0] = first;
}
//jumps to argument if the last instruction was equal
void CPU::do_JE() {
    if (registers[2] == 0) {
        uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
        registers[0] = first;
    }
}
//jumps to argument if the last instruction was not equal
void CPU::do_JNE() {
    if (registers[2] != 0) {
        uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
        registers[0] = first;
    }
}
//pushes next instruction to stack and sets next instruction to argument
void CPU::do_CALL() {
    registers[1] = registers[0];
    uint16_t first = currentInstruction.getArgument(0).read(registers, 2, bus);
    registers[0] = first;
}
//sets next element of the stack as next instruction and adds 2 to the next stack element
void CPU::do_RET() {
    uint16_t first = registers[1];
    registers[1] = registers[1] + 2;
    registers[0] = first;
}
typedef void (CPU::*FunctionPointer)();
//selects the correct methode to do with the operation
uint8_t CPU::processOperation() {
    //array with function pointers of all operations
    FunctionPointer operations[] = {
        do_NOP, do_MOV, do_MOVB, do_PUSH, do_POP, do_ADD,  do_SUB, do_AND, do_OR,  do_XOR,  do_NOT,
        do_SHR, do_SHL, do_INC,  do_DEC,  do_CMP, do_TEST, do_JMP, do_JE,  do_JNE, do_CALL, do_RET};
    //execute correct function corresponding with operation
    std::invoke(operations[currentInstruction.getOperation()], this);
    return 1;
}

void CPU::step() {
    // sets current instruction
    uint8_t i = 0;
    ByteSource source = [this, &i]() { return bus.readOneByte(registers[0] + i++); };
    uint8_t bytesRead = 0;
    
    currentInstruction = Instruction::decode(source, bytesRead);
    // point IP to next instruction
    registers[0] += bytesRead;

    // Execute currentInstruction
    processOperation();
}
