#ifndef SYSPROG_BOTDECISION_H
#define SYSPROG_BOTDECISION_H

#include <queue>
#include "BotModule.h"
#include "./../bot_automaton/bot_automaton.h"

class DecisionOutput : public BotModule {
    private:
        std::queue<uint8_t> decisions;
    public:
        DecisionOutput(bot_addr_t, int);
        void writeOneByte(uint8_t, bot_addr_t);
        void writeTwoBytes(uint16_t, bot_addr_t);
        bool isDecisionAvailable();
        int getDecisionCount();
        uint8_t popDecision();
};

#endif //SYSPROG_BOTDECISION_H
