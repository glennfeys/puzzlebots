#ifndef ACTIVATION_SYSTEM_H
#define ACTIVATION_SYSTEM_H

#include "../system_enums.h"

typedef struct ActivationSystem {
	SystemId system_id;
} ActivationSystem;

#include "../engine.h"

void system_activation_init(ActivationSystem*, Engine*);
ActivationSystem* system_activation_alloc(Engine*);
void system_activation_update(ActivationSystem*, Engine*);
void system_activation_free(ActivationSystem*);



#endif