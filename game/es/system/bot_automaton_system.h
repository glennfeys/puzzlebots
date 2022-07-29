#ifndef PUZZLEBOTS_BOT_AUTOMATON_SYSTEM_H
#define PUZZLEBOTS_BOT_AUTOMATON_SYSTEM_H

#include "../../fsm/fsm.h"
#include "../system_enums.h"

typedef struct BotAutomatonSystem {
    SystemId system_id;
    Fsm* fsm;
    uint8_t* state_table;
} BotAutomatonSystem;

#include "../engine.h"

void system_botautomaton_init(BotAutomatonSystem*, Engine*);
BotAutomatonSystem* system_botautomaton_alloc(Engine*);
void system_botautomaton_update(BotAutomatonSystem*, Engine*);
void system_botautomaton_free(BotAutomatonSystem*);
void move_bot(Engine*, int, EntityId);

#endif // PUZZLEBOTS_BOT_AUTOMATON_SYSTEM_H
