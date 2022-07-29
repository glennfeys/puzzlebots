#include "DecisionOutput.h"
#include <memory>

DecisionOutput::DecisionOutput(bot_addr_t start_address, int sizeInBytes) {
    start = start_address;
    size = sizeInBytes;
}

bool DecisionOutput::isDecisionAvailable() {
    return !decisions.empty();
}

int DecisionOutput::getDecisionCount() {
    return decisions.size();
}

uint8_t DecisionOutput::popDecision() {
    uint8_t tmp = decisions.front();
    decisions.pop();
    return tmp;
}

void DecisionOutput::writeOneByte(uint8_t byte, bot_addr_t location) {
    if (location < start || location >= (start + size)) {
        throw ModuleError("DecisionOuput: cannot write to that memory address!");
    }
    int index = location - IO_DECISION_START;
    if (index != 0) {
        decisions.push(byte);
    }
}

void DecisionOutput::writeTwoBytes(uint16_t bytes, bot_addr_t location) {
    uint8_t first_byte = (uint8_t) (bytes >> 8);
    uint8_t second_byte = (uint8_t) bytes;
    writeOneByte(first_byte, location);
    writeOneByte(second_byte, location + 1);
}
