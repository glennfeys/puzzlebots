#include <utility>

#include "BotBerryExceptions.h"
#include "CPUInstruction.h"
#include "Memory.h"
#include <assert.h>

using namespace CPUInstruction;
// Instruction constructor
Instruction::Instruction(CPUInstruction::Operation newOperation) { operation = newOperation; }
// amount of arguments fo every operation
uint8_t CPUInstruction::operationArguments[] = {0, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1,
                                                2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 0};
void Instruction::addArgument(Argument arg) { arguments[argPos++] = arg; }
Argument Instruction::getArgument(int pos) const { return arguments[pos]; }
Argument* Instruction::getArguments() { return arguments; };
uint8_t Instruction::encode(std::function<void(uint8_t)> byteSink) const {
    // stores the amount of bits written
    uint16_t bitsWritten = 0;
    BitWriter<6> bw;
    // writes operation
    bw.writeBits(operation, 5);
    bitsWritten += 5;
    // for every argument write the type and value
    for (int i = 0; i < operationArguments[operation]; i++) {
        Argument arg = arguments[i];
        uint8_t type = arg.getType();
        // writes type
        bw.writeBits(type, 2);
        bitsWritten += 2;
        if (type == 0 || type == 2) {
            // wites argument
            bw.writeBits(arg.getValue(), 16);
            bitsWritten += 16;
        } else {
            // wtites argument
            bw.writeBits(arg.getValue(), 3);
            bitsWritten += 3;
            if (type == 3) {
                // writes argument
                bw.writeBits(arg.getDisplacement(), 8);
                bitsWritten += 8;
            }
        }
    }
    // writes bites to sink
    bw.toSink(byteSink);
    // transfer bits to bytes
    if (bitsWritten % 8 == 0) {
        bitsWritten = bitsWritten / 8;
    } else {
        bitsWritten = bitsWritten / 8 + 1;
    }
    return bitsWritten;
}

Operation Instruction::getOperation() const { return operation; }
uint16_t Instruction::size() {
    return encode([](uint8_t i) {});
}
Instruction Instruction::decode(std::function<uint8_t()> byteSource, uint8_t& bytes_read) {

    BitReader<6> br(byteSource);
    // read operation
    uint8_t operation = br.readBits<uint8_t>(5);
    if (operation > 21)
        throw DecodingError("The given operation is incorrect");
    bytes_read += 5;
    // make Instruction with operation
    Instruction instruction = Instruction((Operation)operation);
    uint8_t amountOfArguments = operationArguments[operation];
    uint8_t argPos = 0;
    // for each argument in this operation read type and value
    for (uint8_t i = 0; i < amountOfArguments; i++) {
        // read type
        uint8_t operandType = br.readBits<uint8_t>(2);
        bytes_read += 2;
        if (operandType == 0) {
            // read value
            uint16_t immediate = br.readBits<uint16_t>(16);
            bytes_read += 16;
            // sets argument
            instruction.arguments[argPos] = Argument(0, immediate);
        } else if (operandType == 1) {
            // read value
            uint8_t registers = br.readBits<uint8_t>(3);
            if (registers > 8)
                throw DecodingError("The given registerValue is incorrect");
            bytes_read += 3;
            // sets argument
            instruction.arguments[argPos] = Argument(1, registers);
        } else if (operandType == 2) {
            // read value
            uintptr_t address = br.readBits<uintptr_t>(16);
            bytes_read += 16;
            // sets argument
            instruction.arguments[argPos] = Argument(2, address);
        } else {
            // read value
            uint8_t registers = br.readBits<uint8_t>(3);
            if (registers > 8)
                throw DecodingError("The given registerValue is incorrect");
            // read displacement
            uint8_t displacement = br.readBits<uint8_t>(8);
            bytes_read += 11;
            // sets argument
            instruction.arguments[argPos] = Argument(3, registers);
            // sets displacement
            instruction.arguments[argPos].setDisplacement(displacement);
        }
        argPos++;
    }
    // convert bits read to bytes read
    if (bytes_read % 8 == 0) {
        bytes_read = bytes_read / 8;
    } else {
        bytes_read = bytes_read / 8 + 1;
    }
    return instruction;
}
Argument::Argument() : type(NO_ARG), value(0), displacement(0) {}
Argument::Argument(uint8_t type, uint16_t value) : type(type), value(value), displacement(0) {}
uint8_t Argument::getType() const { return type; }
uint16_t Argument::getValue() const { return value; }
void Argument::setDisplacement(uint8_t newDisplacement) { this->displacement = newDisplacement; }
uint8_t Argument::getDisplacement() const { return displacement; }
uint16_t Argument::read(uint16_t CPUregisters[], uint8_t bytes, Bus bus) {
    if (type == 0)
        return value;
    else if (type == 1)
        // return whats in the cpu register[value]
        return CPUregisters[value];
    else if (type == 2) {
        // return whats at the address
        if (bytes == 2)
            return bus.readTwoBytes(value);
        return bus.readOneByte(value);
    } else // type == 3
           // return whats in the register + the displacement
        if (bytes == 2)
        return bus.readTwoBytes(CPUregisters[value] + displacement);
    return bus.readOneByte(CPUregisters[value] + displacement);
}

bool CPUInstruction::operator==(const Instruction& me, const Instruction& other) {
    bool eql = true;
    for (int i = 0; i < 2; i++) {
        if (!(me.getArgument(i) == other.getArgument(i))) {
            eql = false;
            break;
        }
    }
    return me.getOperation() == other.getOperation() && eql;
}
bool CPUInstruction::operator==(const Argument& me, const Argument& other) {
    return me.getType() == other.getType() && me.getValue() == other.getValue() &&
           me.getDisplacement() == other.getDisplacement();
}

uint8_t getBits(uint8_t source, uint8_t pos, uint8_t length) {
    source <<= pos - 1;
    source >>= 8 - length;
    return source;
}
void Argument::write(uint16_t CPUvalue, uint16_t registers[], uint8_t bytes, Bus bus) {
    if (type == 1) // register
        registers[value] = CPUvalue;
    else if (type == 2) { // address
        if (bytes == 2) {
            bus.writeTwoBytes(CPUvalue, value);
        } else {
            bus.writeOneByte(CPUvalue, value);
        }
    } else if (type == 3) // register displacement
        if (bytes == 2) {
            bus.writeTwoBytes(CPUvalue, registers[value] + displacement);
        } else {
            bus.writeOneByte(CPUvalue, registers[value] + displacement);
        }
}
