#include "input_system.h"

#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../level/levelloader.h"
#include "../game_util.h"

#define DISTCHANGE 1.0f

InputSystem* system_input_alloc(Engine* engine) {
    InputSystem* res = calloc(1, sizeof(InputSystem));
    system_input_init(res, engine);
    return res;
}

void system_input_init(InputSystem* system, Engine* engine) { system->system_id = INPUT_SYSTEM; }

void system_input_free(InputSystem* system) {}

static void handleKeyDown(InputSystem* system, Engine* engine, SDL_keysym* keysym,
                          EntityId input_recv_entity_id) {
    if (!has_component(engine, input_recv_entity_id, COMP_ONE_TIME_ANIMATION)) {
        switch (keysym->sym) {
        case SDLK_ESCAPE:
            // ignore untile key released
            break;
        case SDLK_KP_ENTER: // fall-through
        case SDLK_RETURN:   // fall-through
        case SDLK_SPACE: {
            EntityIterator itemaction_it;
            search_entity_2(engine, COMP_ITEMACTION, COMP_CONTAINER, &itemaction_it,
                            system->system_id);
            // for each entity with COMP_ITEMACTION and COMP_CONTAINER
            // set do_action to true
            while (next_entity(&itemaction_it)) {
                ItemActionComponent* itemaction_comp =
                    get_component(engine, itemaction_it.entity_id, COMP_ITEMACTION);
                itemaction_comp->do_action = 1;
            }
            break;
        }
        case SDLK_UP: {
            MoveActionComponent* move_component =
                create_component(engine, input_recv_entity_id, COMP_MOVE_ACTION);
            move_component->is_forward = 1;
            break;
        }
        case SDLK_DOWN: {
            MoveActionComponent* move_component =
                create_component(engine, input_recv_entity_id, COMP_MOVE_ACTION);
            move_component->is_forward = 0;
            break;
        }
        case SDLK_LEFT: {
            DirectionComponent* direction_component =
                get_component(engine, input_recv_entity_id, COMP_DIRECTION);
            direction_component->dir = (direction_component->dir + 1) % DIRECTION_COUNT;
            break;
        }
        case SDLK_RIGHT: {
            DirectionComponent* direction_component =
                get_component(engine, input_recv_entity_id, COMP_DIRECTION);
            direction_component->dir = (direction_component->dir - 1) % DIRECTION_COUNT;
            break;
        }
        case SDLK_PAGEDOWN: {
            EntityIterator input_it;
            EntityId input_eid =
                search_first_entity_1(engine, COMP_INPUTRECEIVER, system->system_id);

            ArtComponent* input_art_comp = get_component(engine, input_eid, COMP_ART);
            // if the focused entity is an eye
            if (input_art_comp->type == ART_EYE) {
                GridLocationComponent* eye_gl_comp =
                    get_component(engine, input_eid, COMP_GRIDLOCATION);

                EntityIterator floor_it;
                search_grid_entity_1(engine, eye_gl_comp->pos, 0, COMP_WALLART, &floor_it,
                                     system->system_id);

                t_ivec3 below_pos;
                glmc_assign_ivec3(below_pos, eye_gl_comp->pos);
                below_pos[ZZ] -= 1;

                EntityIterator itemaction_it;
                search_grid_entity_1(engine, below_pos, 0, COMP_ITEMACTION, &itemaction_it,
                                     system->system_id);

                EntityIterator item_it;
                search_grid_entity_1(engine, below_pos, 0, COMP_ITEM, &item_it, system->system_id);
                // if there is no floor under the eye
                // and there is no bot/tetra/hex underneath the eye
                // move down
                if (!next_entity(&floor_it) && !next_entity(&itemaction_it) &&
                    !next_entity(&item_it)) {
                    create_component(engine, input_eid, COMP_MOVE_ANIMATION);
                    create_component(engine, input_eid, COMP_ONE_TIME_ANIMATION);
                    OneTimeAnimationComponent* onetime_animation =
                        get_component(engine, input_eid, COMP_ONE_TIME_ANIMATION);

                    t_ivec3 direction;
                    glmc_ivec3_set(direction, 0, 0, -1);
                    glmc_assign_ivec3(onetime_animation->destination_pos, direction);
                    onetime_animation->animation_pos = 0;
                    onetime_animation->speed = 500.0;
                    onetime_animation->prev_time = engine->context.time / onetime_animation->speed;
                }
            }
            break;
        }
        case SDLK_PAGEUP: {
            EntityIterator input_it;
            EntityId input_eid =
                search_first_entity_1(engine, COMP_INPUTRECEIVER, system->system_id);

            ArtComponent* input_art_comp = get_component(engine, input_eid, COMP_ART);
            // if the focused entity is an eye move it up
            if (input_art_comp->type == ART_EYE) {
                GridLocationComponent* eye_gl_comp =
                    get_component(engine, input_eid, COMP_GRIDLOCATION);
                create_component(engine, input_eid, COMP_MOVE_ANIMATION);
                create_component(engine, input_eid, COMP_ONE_TIME_ANIMATION);
                OneTimeAnimationComponent* onetime_animation =
                    get_component(engine, input_eid, COMP_ONE_TIME_ANIMATION);

                t_ivec3 direction;
                glmc_ivec3_set(direction, 0, 0, 1);
                glmc_assign_ivec3(onetime_animation->destination_pos, direction);
                onetime_animation->animation_pos = 0;
                onetime_animation->speed = 500.0;
                onetime_animation->prev_time = engine->context.time / onetime_animation->speed;
            }
            break;
        }
        case SDLK_n: {
            // go to next level.
            if (engine->context.currentLevel < LVL_LAST) {
                engine->context.currentLevel++;
            }
            break;
        }
        default:
            break;
        }
    }
}

static void handleKeyUp(InputSystem* system, Engine* engine, SDL_keysym* keysym,
                        EntityId inputReceiverEntity) {
    switch (keysym->sym) {
    case SDLK_ESCAPE:
        engine->context.is_exit_game = 1;
        break;
    case SDLK_UP: {
        break;
    }
    case SDLK_DOWN: {
        break;
    }
    case SDLK_LEFT: {
        break;
    }
    case SDLK_RIGHT: {
        break;
    }
    case SDLK_PAGEDOWN: {
        break;
    }
    case SDLK_PAGEUP: {
        break;
    }
    case SDLK_F1: {
        // switch focus to next bot
        EntityId input_recv_entity_id =
            search_first_entity_1(engine, COMP_INPUTRECEIVER, system->system_id);
        GridLocationComponent* input_location =
            get_component(engine, input_recv_entity_id, COMP_GRIDLOCATION);
        EntityIterator it;
        search_entity_1(engine, COMP_CONTROLLABLE, &it, system->system_id);
        int counter = 0;
        // for each entity with COMP_CONTROLLABLE
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
        // set the new next bot
        engine->context.controlledIndex = (engine->context.controlledIndex + 1) % counter;
        break;
    }
    case SDLK_F5: {
        // switch visibility mode
        engine->context.visibility_mode = (engine->context.visibility_mode + 1) % 3;
        break;
    }
    case SDLK_F2: {
        if (!engine->botautomaton_system->fsm->disabled) {
            engine->context.demo = !(engine->context.demo);
            EntityIterator bot_auto_it;
            search_entity_1(engine, COMP_BOT_AUTOMATON_1, &bot_auto_it, system->system_id);
            while (next_entity(&bot_auto_it)) {
                EntityId bot_auto_id = bot_auto_it.entity_id;
                BotAutomaton1Component* bot_auto_comp =
                    get_component(engine, bot_auto_id, COMP_BOT_AUTOMATON_1);
                bot_auto_comp->is_active = engine->context.demo;
            }
        }
        break;
    }
    case SDLK_F3: {
        engine->context.demo = !(engine->context.demo);
        EntityIterator bot_auto_it;
        search_entity_1(engine, COMP_BOT_AUTOMATON_2, &bot_auto_it, system->system_id);
        while (next_entity(&bot_auto_it)) {
            EntityId bot_auto_id = bot_auto_it.entity_id;
            BotAutomaton2Component* bot_auto_comp =
                get_component(engine, bot_auto_id, COMP_BOT_AUTOMATON_2);
            bot_auto_comp->is_active = engine->context.demo;
        }
    }
    default:
        break;
    }
}

static void handleMouseChange(InputSystem* system, Engine* engine, int8_t mouseChange) {
    CameraLookFromComponent* lookfrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
    if (mouseChange) {
        if (lookfrom->distance > 2.0)
            lookfrom->distance -= DISTCHANGE;
    } else {
        lookfrom->distance += DISTCHANGE;
    }
}

void system_input_update(InputSystem* system, Engine* engine) {
    EntityId input_recv_entity_id =
        search_first_entity_1(engine, COMP_INPUTRECEIVER, system->system_id);

    SDL_Event event;
    memset(&event, 0, sizeof(SDL_Event));
    /* Grab all the events off the queue. */
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handleKeyDown(system, engine, &event.key.keysym, input_recv_entity_id);
            break;
        case SDL_KEYUP:
            /* Handle key release. */
            handleKeyUp(system, engine, &event.key.keysym, input_recv_entity_id);
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            engine->context.is_exit_game = 1;
            break;
        case SDL_MOUSEMOTION: {
            SDL_MouseMotionEvent* mouseMotionEvent = (SDL_MouseMotionEvent*)&event;
            // There are 2 mouse events we want to ignore:
            // - initially, an event is generated that moves the mouse from 0,0 to the current
            // position
            // - then, each time SDL_WarpMouse is called, an event is generated for that call
            if ((mouseMotionEvent->x != mouseMotionEvent->xrel ||
                 mouseMotionEvent->y != mouseMotionEvent->yrel)) {
                int buttonDown = mouseMotionEvent->state & SDL_BUTTON_LEFT;

                if (buttonDown) {
                    CameraLookFromComponent* cameraLookFrom =
                        search_first_component(engine, COMP_CAMERA_LOOK_FROM);
                    if (cameraLookFrom != NULL) {
                        cameraLookFrom->XYdegrees += mouseMotionEvent->xrel * 1.0f;
                        if (cameraLookFrom->Zdegrees - (mouseMotionEvent->yrel / 2.0f) > 0.0 &&
                            cameraLookFrom->Zdegrees - (mouseMotionEvent->yrel / 2.0f) < 90.0) {
                            cameraLookFrom->Zdegrees += -(mouseMotionEvent->yrel / 2.0f);
                        }
                    }
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            // SDL 1.2 handles the wheel in a silly way
            switch (event.button.button) {
            case SDL_BUTTON_WHEELUP: {
                int8_t mouseChange = 1;
                printf("Wheel up\n");
                handleMouseChange(system, engine, mouseChange);
                break;
            }
            case SDL_BUTTON_WHEELDOWN: {
                int8_t mouseChange = 0;
                handleMouseChange(system, engine, mouseChange);
                printf("Wheel down\n");

                break;
            }
            }
            break;
        }
        }
    }
}
