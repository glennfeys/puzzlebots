#ifndef GRAVITY_SYSTEM_H
#define GRAVITY_SYSTEM_H

#include "../system_enums.h"

typedef struct GravitySystem {
	SystemId system_id;
} GravitySystem;

#include "../engine.h"

void system_gravity_init(GravitySystem*, Engine*);
GravitySystem* system_gravity_alloc(Engine*);
void system_gravity_update(GravitySystem*, Engine*);
void system_gravity_free(GravitySystem*);

#endif //GRAVITY_SYSTEM_H
