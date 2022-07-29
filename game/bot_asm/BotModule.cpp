#include "BotModule.h"

uint8_t BotModule::readOneByte(uint16_t) {
    throw ReadNotAllowedError();
}

uint16_t BotModule::readTwoBytes(uint16_t) {
    throw ReadNotAllowedError();
}

void BotModule::writeOneByte(uint8_t, uint16_t) {
    throw WriteNotAllowedError();
}

void BotModule::writeTwoBytes(uint16_t, uint16_t) {
    throw WriteNotAllowedError();
}
