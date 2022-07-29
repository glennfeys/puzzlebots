#ifndef SYSPROG_LED_SYSTEM_H
#define SYSPROG_LED_SYSTEM_H

#include "../system_enums.h"

typedef struct LedSystem {
    //TODO
    SystemId system_id;
} LedSystem;

#include "../engine.h"

void system_led_init(LedSystem*, Engine*);
LedSystem* system_led_alloc(Engine*);
void system_led_update(LedSystem*, Engine*);
void system_led_free(LedSystem*);

#endif //SYSPROG_LED_SYSTEM_H