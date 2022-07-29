#ifndef SYSPROG_BOTMODULE_H
#define SYSPROG_BOTMODULE_H

#include <stdint.h>
#include "BotBerryExceptions.h"
#include "BotBerryConstants.h"

class BotModule {
    //BotModule is in hoofdzaak een "interface", en doet zelf dus weinig.
    //Eventueel mag minimale functionaliteit gedeeld door alle BotModules hier geïmplementeerd worden.
    //Merk op: De CPU zal altijd via Bus gaan, en zal nooit rechtstreeks een BotModule aanspreken.
    //TODO: De nodige puur virtuele methoden definiëren, om 1 of 2 bytes te lezen en schrijven van deze module. Elke concrete BotModule implementatie (Memort, VisiobInput, ...) zal deze methodes moeten implementeren.  
    // id is a int to identify botmodules
    public:
        bot_register_t start; // where the memory adress starts
        int size; // the size of the memory address
        virtual uint8_t readOneByte(bot_addr_t);
        virtual uint16_t readTwoBytes(bot_addr_t);
        virtual void writeOneByte(uint8_t, bot_addr_t);
        virtual void writeTwoBytes(uint16_t, bot_addr_t);
};

#endif //SYSPROG_BOTMODULE_H
