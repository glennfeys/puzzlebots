#ifndef TELEPORT_SYSTEM_H
#define TELEPORT_SYSTEM_H

#include "../system_enums.h"

typedef struct TeleportSystem {
	SystemId system_id;
} TeleportSystem;

#include "../engine.h"

void system_teleport_init(TeleportSystem*, Engine*);
TeleportSystem* system_teleport_alloc(Engine*);
void system_teleport_update(TeleportSystem*, Engine*);
void system_teleport_free(TeleportSystem*);



#endif