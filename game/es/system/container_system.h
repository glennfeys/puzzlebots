#ifndef CONTAINER_SYSTEM_H
#define CONTAINER_SYSTEM_H

#include "../system_enums.h"


typedef struct ContainerSystem {
	SystemId system_id;
} ContainerSystem;

#include "../engine.h"

void system_container_init(ContainerSystem*, Engine*);
ContainerSystem* system_container_alloc(Engine*);
void system_container_update(ContainerSystem*, Engine*);
void system_container_free(ContainerSystem*);


#endif //CONTAINER_SYSTEM_H