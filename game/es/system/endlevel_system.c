#include "endlevel_system.h"
#include "input_system.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

EndLevelSystem* system_endlevel_alloc(Engine* engine) {
    EndLevelSystem* res = calloc(1, sizeof(EndLevelSystem));
    system_endlevel_init(res, engine);
    return res;
}

void system_endlevel_init(EndLevelSystem* system, Engine* engine) {
    system->system_id = ENDLEVEL_SYSTEM;
}

void system_endlevel_free(EndLevelSystem* system) {}

void system_endlevel_update(EndLevelSystem* system, Engine* engine) {
    int all_finished = 1;

    EntityIterator exitaffected_it;
    search_entity_1(engine, COMP_EXIT_AFFECTED, &exitaffected_it, system->system_id);
    //for each entity with COMP_EXIT_AFFECTED
    while (next_entity(&exitaffected_it)) {
        EntityId exitaffected_entity_id = exitaffected_it.entity_id;

        GridLocationComponent* exitaffected_location =
            get_component(engine, exitaffected_entity_id, COMP_GRIDLOCATION);
        ExitAffectedComponent* exitaffected_exa =
            get_component(engine, exitaffected_entity_id, COMP_EXIT_AFFECTED);
        ContainerComponent* container_comp =
            get_component(engine, exitaffected_entity_id, COMP_CONTAINER);

        // if all bots need to be finnished to finnish the level set all_finnished to false
        if (exitaffected_exa->required_for_level_finish) {
            all_finished = 0;
        }

        EntityIterator exit_it;
        search_grid_entity_1(engine, exitaffected_location->pos, 1, COMP_EXIT, &exit_it,
                             system->system_id);
        //if there is a entity with COMP_EXIT on location exitaffected_location->pos
        if (next_entity(&exit_it)) {
            //if an entity is exit affected and does not have an item in it's container
            //then start the exit animation
            if (!has_component(engine, exitaffected_entity_id, COMP_EXITED) &&
                container_comp->item == NO_ENTITY) {
                create_component(engine, exitaffected_entity_id, COMP_EXITED);
                create_component(engine, exitaffected_entity_id, COMP_MOVE_ANIMATION);
                create_component(engine, exitaffected_entity_id, COMP_ONE_TIME_ANIMATION);
                OneTimeAnimationComponent* onetime_animation =
                    get_component(engine, exitaffected_entity_id, COMP_ONE_TIME_ANIMATION);
                t_ivec3 vector;
                glmc_ivec3_set(vector, 0, 0, 3);
                glmc_assign_ivec3(onetime_animation->destination_pos, vector);
                onetime_animation->animation_pos = 0;
                onetime_animation->speed = 400.0;
                onetime_animation->prev_time = engine->context.time / onetime_animation->speed;
                free_component(engine, exitaffected_entity_id, COMP_GRAVITY);
            }
            //if the exit animation is over, delete the entity and shift the focus.
            if (!has_component(engine, exitaffected_entity_id, COMP_ONE_TIME_ANIMATION) &&
                has_component(engine, exitaffected_entity_id, COMP_EXITED)) {
                EntityId focused_id =
                    search_first_entity_1(engine, COMP_INPUTRECEIVER, system->system_id);
                if (focused_id == exitaffected_entity_id) {
                    next_bot(engine, system);
                }
                delete_entity(engine, exitaffected_entity_id);
                engine->context.controlledIndex--;
            }
        }
    }
    //if all bots are finnished, exit the game.
    if (all_finished) {
        if (engine->context.currentLevel < LVL_LAST) {
            engine->context.currentLevel++;
        } else {
            engine->context.is_exit_game = 1;
        }
    }
}

void next_bot(Engine* engine, EndLevelSystem* system) {
    //This gives the next bot focus
    EntityId input_recv_entity_id =
        search_first_entity_1(engine, COMP_INPUTRECEIVER, system->system_id);
    GridLocationComponent* input_location =
        get_component(engine, input_recv_entity_id, COMP_GRIDLOCATION);
    EntityIterator it;
    search_entity_1(engine, COMP_CONTROLLABLE, &it, system->system_id);
    int counter = 0;
    //for each entity with COMP_CONTROLLABLE
    while (next_entity(&it)) {
        // if this is the next bot, give it focus
        if (counter == engine->context.controlledIndex) {
            free_component(engine, input_recv_entity_id, COMP_INPUTRECEIVER);
            free_component(engine, input_recv_entity_id, COMP_CAMERA_LOOK_AT);
            create_component(engine, it.entity_id, COMP_INPUTRECEIVER);
            create_component(engine, it.entity_id, COMP_CAMERA_LOOK_AT);
            CameraLookAtComponent* cam_comp =
                get_component(engine, it.entity_id, COMP_CAMERA_LOOK_AT);
            cam_comp->last_update = 0;
            glmc_assign_vec3_from_ivec3(cam_comp->pos, input_location->pos);
        }
        counter++;
    }
    //set the new next bot
    engine->context.controlledIndex = (engine->context.controlledIndex + 1) % counter;
}
