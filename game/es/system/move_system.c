#include "move_system.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_MOVE_MS 250u

MoveSystem* system_move_alloc(Engine* engine) {
    MoveSystem* res = calloc(1, sizeof(MoveSystem));
    system_move_init(res, engine);
    return res;
}

void system_move_init(MoveSystem* system, Engine* engine) { system->system_id = MOVE_SYSTEM; }

void system_move_free(MoveSystem* system) {}

void system_move_update(MoveSystem* system, Engine* engine) {
    EntityIterator moving_entities;
    search_entity_1(engine, COMP_MOVE_ACTION, &moving_entities, system->system_id);
    //for each entity with COMP_MOVE_ACTION
    while (next_entity(&moving_entities)) {
        EntityId moving_entity_id = moving_entities.entity_id;

        DirectionComponent* direction_component =
            get_component(engine, moving_entity_id, COMP_DIRECTION);
        MoveActionComponent* move_component =
            get_component(engine, moving_entity_id, COMP_MOVE_ACTION);

        t_ivec3 direction;
        //get the direction vector.
        switch (direction_component->dir) {
        case 0: { // North
            glmc_ivec3_set(direction, 0, 1, 0);
            break;
        }
        case 1: { // West
            glmc_ivec3_set(direction, -1, 0, 0);
            break;
        }
        case 2: { // South
            glmc_ivec3_set(direction, 0, -1, 0);
            break;
        }
        case 3: { // East
            glmc_ivec3_set(direction, 1, 0, 0);
            break;
        }
        default:
            break;
        }
        // Check if the bit is moving forward.
        if (!move_component->is_forward) {
            direction[0] = -direction[0];
            direction[1] = -direction[1];
        }
        //get the new location
        t_ivec3 new_pos;
        GridLocationComponent* old_location =
            get_component(engine, moving_entity_id, COMP_GRIDLOCATION);
        glmc_assign_ivec3(new_pos, old_location->pos);
        new_pos[0] += direction[0];
        new_pos[1] += direction[1];
        new_pos[2] += direction[2];

        EntityIterator walkable_it;
        //true if new_pos is walkable
        uint8_t is_walkable = 1;
        //true if new_pos is not just void
        uint8_t has_walkable = 0;
        search_grid_entity_1(engine, new_pos, 1, COMP_GRIDLOCATION, &walkable_it,
                             system->system_id);
        //for each entity with gridlocation on new_pos
        while (next_entity(&walkable_it)) {
            has_walkable = 1;
            EntityId walkable_id = walkable_it.entity_id;
            GridLocationComponent* location_comp =
                get_component(engine, walkable_id, COMP_GRIDLOCATION);
            ArtComponent* art_comp = get_component(engine, moving_entity_id, COMP_ART);
            WalkComponent* walk_comp = get_component(engine, walkable_id, COMP_WALKABLE);
            //if it has no walkable
            if (walk_comp == NULL) {
                // if the next block doesnt go up
                if (old_location->pos[2] - location_comp->pos[2] < 0) {
                    is_walkable = 0;
                }
            } else if (art_comp->type != ART_EYE) { //if it has a walkable comp

                // the amount the walkable is lower (-1 lower = 1 higher)
                int drop =
                    old_location->pos[2] - (location_comp->pos[2] + walk_comp->walkHeight);
                // if walkable is higher
                if (drop < 0) {
                    // if walkable is one higher and pushable
                    if (drop == -1 && has_component(engine, walkable_id, COMP_PUSHABLE)) {
                        //true if you can push the entity
                        uint8_t is_pushable = 0;
                        //amount of pushables before bot.
                        uint8_t amount_of_pushables = 1;
                        t_ivec3 next_pos;
                        glmc_assign_ivec3(next_pos, new_pos);
                        // count howmany pushables are before the bot.
                        while (!is_pushable && amount_of_pushables < 3) {
                            //set next_position to the next position a pashable can be.
                            glmc_ivec3_set(next_pos, next_pos[0] + direction[0],
                                            next_pos[1] + direction[1],
                                            next_pos[2] + direction[2]);
                            EntityIterator pushable_it;
                            search_grid_entity_1(engine, next_pos, 0, COMP_PUSHABLE,
                                                    &pushable_it, system->system_id);
                            //if the next_position has no more pushable
                            //check if it is a location you can push something to
                            if (!next_entity(&pushable_it)) {
                                EntityIterator items_it;
                                search_grid_entity_1(engine, next_pos, 0, COMP_WALKABLE,
                                                        &items_it, system->system_id);
                                // if there are walkables
                                if (next_entity(&items_it)) {
                                    WalkComponent* wc = get_component(
                                        engine, items_it.entity_id, COMP_WALKABLE);
                                    //if walkheight is 0 you can push over it.
                                    if (wc->walkHeight == 0) {
                                        is_pushable = 1;
                                    }
                                } else { //if there are no walkables infront
                                    EntityIterator items2_it;
                                    search_grid_entity_1(engine, next_pos, 1, COMP_GRIDLOCATION,
                                                            &items2_it, system->system_id);
                                    //if there is something with a gridlocation in pront wich is lower than the pushable.
                                    if (next_entity(&items2_it)) {
                                        GridLocationComponent* gc = get_component(
                                            engine, items2_it.entity_id, COMP_GRIDLOCATION);
                                        if (gc->pos[2] <= next_pos[2]) {
                                            //you can push it
                                            is_pushable = 1;
                                        }
                                    }
                                }
                                break;
                            } else { // if it has another pushable in fornt of it
                                amount_of_pushables++;
                            }
                        }
                        //if you can push, push it
                        if (is_pushable) {
                            MoveActionComponent* mc =
                                create_component(engine, walkable_id, COMP_MOVE_ACTION);
                            mc->is_forward = move_component->is_forward;
                            DirectionComponent* dc =
                                get_component(engine, walkable_id, COMP_DIRECTION);
                            dc->dir = direction_component->dir;
                            PushableComponent* push_comp =
                                get_component(engine, walkable_id, COMP_PUSHABLE);
                        } else { // if you can't push it do not let the bot walk
                            is_walkable = 0;
                        }

                    } else { //if the drop is deeper than -1 or it is not a pushable (ex. a door)
                        //you can't walk
                        is_walkable = 0;
                    }
                }
            }
        }
        //if new_pos is void, you can't walk there
        if (!has_walkable) {
            is_walkable = 0;
        }
        //if we can move it, and it does't already move, move
        if (!has_component(engine, moving_entity_id, COMP_ONE_TIME_ANIMATION) && is_walkable) {
            create_component(engine, moving_entity_id, COMP_MOVE_ANIMATION);
            create_component(engine, moving_entity_id, COMP_ONE_TIME_ANIMATION);
            OneTimeAnimationComponent* onetime_animation =
                get_component(engine, moving_entity_id, COMP_ONE_TIME_ANIMATION);
            glmc_assign_ivec3(onetime_animation->destination_pos, direction);
            onetime_animation->animation_pos = 0;
            onetime_animation->speed = 500.0;
            onetime_animation->prev_time = engine->context.time / onetime_animation->speed;
        }
        free_component(engine, moving_entity_id, COMP_MOVE_ACTION);
    }
}
