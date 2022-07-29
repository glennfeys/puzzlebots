#include "Memory.h"

Memory::Memory(bot_register_t start_address, int sizeInBytes) {
    memory = new uint8_t[sizeInBytes];
    size = sizeInBytes;
    start = start_address;
}

Memory::~Memory() {
    delete [] memory;
}

uint8_t Memory::readOneByte(bot_addr_t location) {
    location = location - start;
    if (location >= size) {
        throw ModuleError("The given address is out of this memory range!");
    }
    return memory[location];
}

uint16_t Memory::readTwoBytes(bot_addr_t location) {
    uint16_t bytes = readOneByte(location) << 8;
    return bytes | readOneByte(location + 1);
}

void Memory::writeOneByte(uint8_t byte, bot_addr_t location) {
    location = location - start;
    if (location >= size) {
        throw ModuleError("The given address is out of this memory range!");
    }
    memory[location] = byte;
}

void Memory::writeTwoBytes(uint16_t bytes, bot_addr_t location) {
    writeOneByte((uint8_t) bytes, location + 1);
    bytes = bytes >> 8;
    writeOneByte((uint8_t) bytes, location);
}
