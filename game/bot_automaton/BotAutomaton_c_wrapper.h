#ifndef SYSPROG_BOTAUTOMATON_C_WRAPPER_H
#define SYSPROG_BOTAUTOMATON_C_WRAPPER_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BotBerri {
    void* obj;
} BotBerri;

BotBerri* createBotBerri(uint16_t size);
void freeBotBerri(BotBerri* botberri);
void initBotBerri(BotBerri* botberri);

void doCPUStep(BotBerri* botberri);
void setVisionMask(BotBerri* botberri, uint32_t mask);
uint8_t getNextMove(BotBerri* botberri);
int isDecisionAvailable(BotBerri* botberri);
void loadFSM(BotBerri* botberri, uint8_t* fsm_tabel);

#ifdef __cplusplus
}
#endif

#endif // SYSPROG_BOTAUTOMATON_C_WRAPPER_H
