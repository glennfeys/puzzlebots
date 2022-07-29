#include "gravity_system.h"
#include "move_system.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_FALL_MS 100u

GravitySystem* system_gravity_alloc(Engine* engine) {
    GravitySystem* res = calloc(1, sizeof(GravitySystem));
    system_gravity_init(res, engine);
    return res;
}

void system_gravity_init(GravitySystem* system, Engine* engine) {
    system->system_id = GRAVITY_SYSTEM;
}

void system_gravity_free(GravitySystem* system) {}

void system_gravity_update(GravitySystem* system, Engine* engine) {
    EntityIterator gravity_entities;
    search_entity_1(engine, COMP_GRAVITY, &gravity_entities, system->system_id);
    //for each entity with COMP_GRAVITY
    while (next_entity(&gravity_entities)) {
        EntityId gravity_entity_id = gravity_entities.entity_id;
        GridLocationComponent* entity_location =
            get_component(engine, gravity_entity_id, COMP_GRIDLOCATION);

        EntityIterator other_pos_entities;
        search_grid_entity_1(engine, entity_location->pos, 1, COMP_GRIDLOCATION,
                             &other_pos_entities, system->system_id);
        int smallest_drop = -255; // biggest drop possible
        //for each entity with COMP_GRIDLOCATION on the same position as the gravity entity
        //check what the smallest drop is they can make
        while (next_entity(&other_pos_entities)) {
            EntityId other_id = other_pos_entities.entity_id;
            if (other_id != gravity_entity_id) {
                GridLocationComponent* other_entity_location =
                    get_component(engine, other_id, COMP_GRIDLOCATION);
                WalkComponent* walk_component = get_component(engine, other_id, COMP_WALKABLE);
                //if the other entity is not walkable
                if (walk_component != NULL) {
                    //check if the drop is smaller than our current smallest drop.
                    if (smallest_drop < other_entity_location->pos[ZZ] - entity_location->pos[ZZ] +
                                            walk_component->walkHeight) {
                        smallest_drop = other_entity_location->pos[ZZ] - entity_location->pos[ZZ] +
                                        walk_component->walkHeight;
                    }
                } else { //if the other entity is not walkable
                    //check if the drop is smaller than our current smallest drop.
                    if (smallest_drop < other_entity_location->pos[ZZ] - entity_location->pos[ZZ]) {
                        smallest_drop = other_entity_location->pos[ZZ] - entity_location->pos[ZZ];
                    }
                }
            }
        }
        //if there is a drop and there is no animation.
        //fall down the amount of the smallest drop with animation.
        if (!has_component(engine, gravity_entity_id, COMP_ONE_TIME_ANIMATION) &&
            smallest_drop < 0) {
            create_component(engine, gravity_entity_id, COMP_MOVE_ANIMATION);
            create_component(engine, gravity_entity_id, COMP_ONE_TIME_ANIMATION);
            OneTimeAnimationComponent* onetime_animation =
                get_component(engine, gravity_entity_id, COMP_ONE_TIME_ANIMATION);
            t_ivec3 vector;
            glmc_ivec3_set(vector, 0, 0, smallest_drop);
            glmc_assign_ivec3(onetime_animation->destination_pos, vector);
            onetime_animation->animation_pos = 0;
            onetime_animation->speed = 200.0;
            onetime_animation->prev_time = engine->context.time / onetime_animation->speed;
        }
    }
}
