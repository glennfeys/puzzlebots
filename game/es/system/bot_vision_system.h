#ifndef PUZZLEBOTS_BOT_SENSE_H
#define PUZZLEBOTS_BOT_SENSE_H

#include "../../bot_automaton/bot_automaton.h"
#include "../system_enums.h"
#include <stdint.h>

typedef struct BotVisionSystem {
    SystemId system_id;
} BotVisionSystem;

#include "../engine.h"

void system_botvision_init(BotVisionSystem*, Engine*);
BotVisionSystem* system_botvision_alloc(Engine*);
void system_botvision_update(BotVisionSystem*, Engine*);
void system_botvision_free(BotVisionSystem*);
void set_bitmask(uint32_t*, uint8_t);
void get_direction(t_ivec2, uint8_t);

#endif // PUZZLEBOTS_BOT_SENSE_H
