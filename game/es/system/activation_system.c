#include "activation_system.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

ActivationSystem* system_activation_alloc(Engine* engine) {
    ActivationSystem* res = calloc(1, sizeof(ActivationSystem));
    system_activation_init(res, engine);
    return res;
}

void system_activation_init(ActivationSystem* system, Engine* engine) {
    system->system_id = ACTIVATION_SYSTEM;
}

void system_activation_free(ActivationSystem* system) {}

void system_activation_update(ActivationSystem* system, Engine* engine) {
    EntityIterator act_it;
    search_entity_2(engine, COMP_ACTIVATION, COMP_BUTTON, &act_it, system->system_id);
    //for each entity with COMP_ACTIVATION and COMP_BUTTON
    while (next_entity(&act_it)) {
        EntityId act_eid = act_it.entity_id;

        ActivationComponent* act_comp = get_component(engine, act_eid, COMP_ACTIVATION);
        EntityIterator condact_it;
        search_entity_1(engine, COMP_CONDITIONAL_ACTIVATION, &condact_it, system->system_id);
        //for each entity with COMP_CONDITIONAL_ACTIVATION
        while (next_entity(&condact_it)) {
            EntityId condact_eid = condact_it.entity_id;

            ConditionalActivationComponent* condact_comp =
                get_component(engine, condact_eid, COMP_CONDITIONAL_ACTIVATION);
            //if the activation id's are the same, open/close the door
            if (condact_comp->activation_id == act_comp->activation_id) {
                ActivationComponent* door_act_comp =
                    get_component(engine, condact_eid, COMP_ACTIVATION);
                WalkComponent* walk_comp = get_component(engine, condact_eid, COMP_WALKABLE);
                if (act_comp->active) { // Open the door
                    door_act_comp->active = 1;
                    walk_comp->walkHeight = 0;
                } else { //Close the door
                    door_act_comp->active = 0;
                    walk_comp->walkHeight = 1;
                }
            }
        }
    }
}
