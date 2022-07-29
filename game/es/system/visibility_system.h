#ifndef VISIBILITY_SYSTEM_H
#define VISIBILITY_SYSTEM_H

#include "../system_enums.h"

typedef struct VisibilitySystem {
	SystemId system_id;
} VisibilitySystem;

#include "../engine.h"

void system_visibility_init(VisibilitySystem*, Engine*);
VisibilitySystem* system_visibility_alloc(Engine*);
void system_visibility_update(VisibilitySystem*, Engine*);
void system_visibility_free(VisibilitySystem*);

#endif //VISIBILITY_SYSTEM_H