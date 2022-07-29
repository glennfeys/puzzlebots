#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <stdint.h>
#include "../system_enums.h"

typedef struct InputSystem {
    SystemId system_id;
} InputSystem;

#include "../engine.h"

void system_input_init(InputSystem*, Engine*);
InputSystem* system_input_alloc(Engine*);
void system_input_update(InputSystem*, Engine*);
void system_input_free(InputSystem*);

#endif //INPUT_SYSTEM_H