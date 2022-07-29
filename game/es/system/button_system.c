#include "button_system.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

ButtonSystem* system_button_alloc(Engine* engine) {
    ButtonSystem* res = calloc(1, sizeof(ButtonSystem));
    system_button_init(res, engine);
    return res;
}

void system_button_init(ButtonSystem* system, Engine* engine) { system->system_id = BUTTON_SYSTEM; }

void system_button_free(ButtonSystem* system) {}

void system_button_update(ButtonSystem* system, Engine* engine) {
    EntityIterator button_it;
    search_entity_1(engine, COMP_BUTTON, &button_it, system->system_id);
    //for each entity with COMP_BUTTON
    while (next_entity(&button_it)) {
        EntityId button_eid = button_it.entity_id;

        ActivationComponent* buttonact_comp = get_component(engine, button_eid, COMP_ACTIVATION);
        ButtonComponent* button_comp = get_component(engine, button_eid, COMP_BUTTON);
        GridLocationComponent* buttongl_comp = get_component(engine, button_eid, COMP_GRIDLOCATION);

        EntityIterator activator_it;
        search_grid_entity_1(engine, buttongl_comp->pos, 1, COMP_BUTTON_ACTIVATOR, &activator_it,
                             system->system_id);
        //if there is an activator on the button
        int bot_on_button = next_entity(&activator_it);
        //if a button that requires hold is active but there is no longer an activator on there, unactivate it
        if (buttonact_comp->active && button_comp->requires_hold && !bot_on_button) {
            buttonact_comp->active = 0;
        //if there is an unactive button button and there is an activator on it, activate it.
        } else if (!buttonact_comp->active && bot_on_button) {
            buttonact_comp->active = 1;
        }
    }
}
