#ifndef BUTTON_SYSTEM_H
#define BUTTON_SYSTEM_H

#include "../system_enums.h"

typedef struct ButtonSystem {
	SystemId system_id;
} ButtonSystem;

#include "../engine.h"

void system_button_init(ButtonSystem*, Engine*);
ButtonSystem* system_button_alloc(Engine*);
void system_button_update(ButtonSystem*, Engine*);
void system_button_free(ButtonSystem*);



#endif