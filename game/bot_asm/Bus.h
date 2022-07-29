#ifndef SYSPROG_BOTBUS_H
#define SYSPROG_BOTBUS_H

#include <memory>
#include <map>
#include "BotModule.h"

#include <cstdint>

class Bus {
    //TODO: Hou in deze klasse een lijst BotModules bij. Doe dit generiek: Ga er niet van uit dat Bus op voorhand weet welke modules op welk geheugenadres beschikbaar gemaakt zullen worden.
    //TODO: Implementeer een methode om een BotModule (std::shared_ptr<BotModule>) toe te voegen aan de Bus
    //TODO: Implementeer methodes om 1 of 2 bytes te lezen en schrijven van een bepaald geheugenadres. De Bus zal dan a.d.h.v het geheugenadres de juiste module contacteren. Deze methodes worden door de CPU opgeroepen. De CPU zal nooit rechtstreeks een BotModule aanspreken.
    private:
        // map that has as keys the start adresses and as values smart pointers to a botmodule
        std::map<bot_addr_t, std::shared_ptr<BotModule>> modules_map;
        std::shared_ptr<BotModule> getRightBotModule(bot_addr_t);
        
    public:
        void addBotModule(std::shared_ptr<BotModule>);
        uint8_t readOneByte(uint16_t);
        uint16_t readTwoBytes(uint16_t);
        void writeOneByte(uint8_t, uint16_t);
        void writeTwoBytes(uint16_t, uint16_t);
};

// Dit is door Glenn ingevoegd en is dus niet zeker/definitief

#endif // SYSPROG_BOTBUS_H
