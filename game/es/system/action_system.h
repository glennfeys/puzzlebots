#ifndef ACTION_SYSTEM_H
#define ACTION_SYSTEM_H

#include "../system_enums.h"

typedef struct ActionSystem {
    SystemId systemId;
} ActionSystem;

#include "../engine.h"

void system_action_init(ActionSystem*, Engine*);
ActionSystem* system_action_alloc(Engine*);
void system_action_update(ActionSystem*, Engine*);
void system_action_free(ActionSystem*);



#endif
