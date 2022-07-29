#include "VisionInput.h"

VisionInput::VisionInput(bot_addr_t start_address, int sizeInBytes) {
    start = start_address;
    size = sizeInBytes;
    vision_mask = new uint8_t[4];
}

// set the vision bitmask
void VisionInput::setVisionBitMask(uint32_t mask) {
    for (int i = 0; i < 4; i++) {
        vision_mask[3 - i] = (uint8_t)(mask >> (i * 8));
    }
}

// this method reads 1 byte from the bot vision bitmask
uint8_t VisionInput::readOneByte(bot_addr_t location) {
    if (location < start || location >= (start + size)) {
        throw ModuleError("VisionInput: cannot read that memory address!");
    }
    // vision position: 0 = front, 1 = left, 2 = right, 3 = current
    int vision_position = location - start;
    return vision_mask[vision_position];
}

uint16_t VisionInput::readTwoBytes(bot_addr_t location) {
    uint16_t first_byte = readOneByte(location);
    uint8_t second_byte = readOneByte(location + 1);
    return (first_byte << 8) | second_byte;
}
VisionInput::~VisionInput() { delete[] vision_mask; }
