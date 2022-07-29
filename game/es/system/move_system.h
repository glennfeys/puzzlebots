#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include "../system_enums.h"

typedef struct MoveSystem {
	SystemId system_id;
} MoveSystem;

#include "../engine.h"

void system_move_init(MoveSystem*, Engine*);
MoveSystem* system_move_alloc(Engine*);
void system_move_update(MoveSystem*, Engine*);
void system_move_free(MoveSystem*);

#endif //MOVE_SYSTEM_H
