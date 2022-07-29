#ifndef SYSPROG_BOTMEM_H
#define SYSPROG_BOTMEM_H

#include <vector>
#include "BotModule.h"

class Memory : public BotModule {
    //TODO Implementeer een constructor, die (o.a.) de grootte van dit blok geheugen mee krijgt.
    //TODO Implementeer de virtuele methoden van BotModule, om 1 of 2 bytes te lezen en schrijven uit/naar dit blok geheugen.
    private:
        uint8_t* memory;
    public:
        Memory(bot_register_t, int);
        ~Memory();
        uint8_t readOneByte(bot_addr_t);
        uint16_t readTwoBytes(bot_addr_t);
        void writeOneByte(uint8_t, uint16_t);
        void writeTwoBytes(uint16_t, uint16_t);
};

#endif //SYSPROG_BOTMEM_H
