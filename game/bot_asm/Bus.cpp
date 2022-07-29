#include "Bus.h"

// method to add a new botmodule
// first check if the address is valid then put it in the map
void Bus::addBotModule(std::shared_ptr<BotModule> module) {
    if (module->start < USABLE_ADDRESS_START) {
        throw BusError("The given module has a to small start address!");
    }
    // this checks if the module you're adding doesn't share the same addresses
    std::map<bot_addr_t, std::shared_ptr<BotModule>>::iterator it;
    for (it = modules_map.begin(); it != modules_map.end(); it++) {
        bot_addr_t start = it->first;
        int size = it->second->size;
        // x1 <= y2 && y1 <= x2 formula to test if 2 ranges [x1, x2], [y1, y2] are overlaping
        if (start < (module->size + module->start) && module->start < (size + start)) {
            throw BusError("Memory address is already taken by an other module!");
        }
    }
    modules_map.emplace(module->start, module);
}   

std::shared_ptr<BotModule> Bus::getRightBotModule(bot_addr_t location) {
    // iterator to loop over the map of start addresses and botmodules
    std::map<bot_addr_t, std::shared_ptr<BotModule>>::iterator it;
    for (it = modules_map.begin(); it != modules_map.end(); it++) {
        bot_addr_t start = it->first;
        int size = it->second->size;
        if (start <= location && location < (start + size)) {
            return it->second;
        }
    }
    throw BusError("Unreachable memory address!"); 
}

uint8_t Bus::readOneByte(bot_addr_t location) {
    return getRightBotModule(location)->readOneByte(location);
}

uint16_t Bus::readTwoBytes(bot_addr_t location) {
    return getRightBotModule(location)->readTwoBytes(location);
}

void Bus::writeOneByte(uint8_t byte, bot_addr_t location) {
    getRightBotModule(location)->writeOneByte(byte, location);
}

void Bus::writeTwoBytes(uint16_t bytes, bot_addr_t location) {
    getRightBotModule(location)->writeTwoBytes(bytes, location);
}
