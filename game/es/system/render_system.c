#include "render_system.h"
#include "../../../util/util.h"
#include "../../../util/rgb_triple.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

RenderSystem* system_render_alloc(Graphics* graphics, Engine* engine) {
    RenderSystem* res = calloc(1, sizeof(RenderSystem));
    system_render_init(res, graphics, engine);
    return res;
}

void system_render_init(RenderSystem* render_system, Graphics* graphics, Engine* engine) {
    render_system->system_id = RENDER_SYSTEM;

    render_system->graphics = graphics;

    gl_tetrahedron_init(&render_system->tetrahedron, graphics);
    gl_hexprism_init(&render_system->hexprism, graphics);
    gl_floor_init(&render_system->floor, graphics);
    gl_exit_init(&render_system->exit, graphics);
    gl_wall_init(&render_system->wall[N], graphics, N);
    gl_wall_init(&render_system->wall[S], graphics, S);
    gl_wall_init(&render_system->wall[E], graphics, E);
    gl_wall_init(&render_system->wall[W], graphics, W);
    gl_actor_init(&render_system->bot, graphics, GLACTOR_BOT_SCALE);
    gl_actor_init(&render_system->eye, graphics, GLACTOR_EYE_SCALE);
    gl_button_init(&render_system->button, graphics);
    gl_door_init(&render_system->door[X], graphics, X);
    gl_door_init(&render_system->door[Y], graphics, Y);
    gl_teleport_init(&render_system->teleport, graphics);
}

void system_render_free(RenderSystem* render_system) {
    gl_tetrahedron_free(&render_system->tetrahedron);
    gl_hexprism_free(&render_system->hexprism);
    gl_floor_free(&render_system->floor);
    gl_exit_free(&render_system->exit);
    gl_wall_free(&render_system->wall[N]);
    gl_wall_free(&render_system->wall[S]);
    gl_wall_free(&render_system->wall[E]);
    gl_wall_free(&render_system->wall[W]);
    gl_actor_free(&render_system->bot);
    gl_actor_free(&render_system->eye);
    gl_button_free(&render_system->button);
    gl_door_free(&render_system->door[X]);
    gl_door_free(&render_system->door[Y]);
}

static void set_camera(RenderSystem* system, Engine* engine) {
    CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
    CameraLookAtComponent* cameraLookAt = search_first_component(engine, COMP_CAMERA_LOOK_AT);
    if (cameraLookFrom != NULL) {
        glmc_assign_vec3(system->graphics->cameraLookFrom, cameraLookFrom->pos);
        glmc_assign_vec3(engine->context.lastCameraLookFrom, cameraLookFrom->pos);
    } else {
        fprintf(stderr, "cameraLookFrom not found. Using last know camera positions\n");
        glmc_assign_vec3(system->graphics->cameraLookFrom, engine->context.lastCameraLookFrom);
    }
    
    if (cameraLookAt != NULL) {
        glmc_assign_vec3(system->graphics->cameraLookAt, cameraLookAt->pos);
        glmc_assign_vec3(engine->context.lastCameraLookAt, cameraLookAt->pos);
    } else {
        fprintf(stderr, "cameraLookAt not found. Using last know camera positions\n");
        glmc_assign_vec3(system->graphics->cameraLookAt, engine->context.lastCameraLookAt);
    }
    
    t_vec3 upDown = { 0, 1, 0 };
    glmc_assign_vec3(system->graphics->cameraUp, upDown);
}

void system_render_update(RenderSystem* system, Engine* engine) {
    set_camera(system, engine);
    
    uint64_t time = engine->context.time;

    graphics_begin_draw(system->graphics);
    
    EntityIterator draw_it;
    search_entity_2(engine, COMP_GRIDLOCATION, COMP_ART, &draw_it, system->system_id);
    
    while(next_entity(&draw_it)) {
        EntityId drawable_entity_id = draw_it.entity_id;
        
        GridLocationComponent* grid = get_component(engine, drawable_entity_id, COMP_GRIDLOCATION);
        ArtComponent* art = get_component(engine, drawable_entity_id, COMP_ART);
    
        VisibleComponent *visibility_comp = get_component(engine, drawable_entity_id, COMP_VISIBLE);
    
        int is_shaded = 0;
        if (engine->context.visibility_mode != ALL) {
            if (visibility_comp == NULL)
                continue;
            is_shaded = visibility_comp->is_shaded;
            if (engine->context.visibility_mode == ONLY_DIRECT && is_shaded)
                continue;
        }
        
        t_vec3 grid_pos = {0};
        if (has_component(engine, drawable_entity_id, COMP_MOVE_ANIMATION)){
            MoveAnimationComponent* move_animation = get_component(engine, drawable_entity_id, COMP_MOVE_ANIMATION);
            glmc_assign_vec3(grid_pos, move_animation->curr_pos);
        }
        else {
            glmc_assign_vec3_from_ivec3(grid_pos, grid->pos);
        }
        
        //if there is an animation, use that instead
//        MoveAnimationComponent* move_anim = get_component(engine, drawable_entity_id, COMP_MOVE_ANIMATION);
//        if (move_anim != NULL && move_anim->position < 1.0f) {
//            glmc_assign_vec3(grid_pos, move_anim->cur);
//        }
        
        switch (art->type) {
            case ART_WALL: {
                WallArtComponent *wallInfo = get_component(engine, drawable_entity_id, COMP_WALLART);
                
                assert(wallInfo != NULL);
                if (wallInfo != NULL) {
                    for (Direction dir = DIRECTION_FIRST; dir <= DIRECTION_LAST; dir++) {
                        assert(wallInfo->wall_height[dir] >= 0);
                        assert(wallInfo->wall_height[dir] < 10);
                        for (int z = 0; z < wallInfo->wall_height[dir]; z++) {
                            t_vec3 wall_pos = {0};
                            glmc_vec3_set(wall_pos, grid_pos[0], grid_pos[1], grid_pos[2] + z);
                            gl_wall_draw_color(&system->wall[dir], wall_pos, is_shaded);
                        }
                    }
                    
                    if (wallInfo->has_floor) {
                        gl_floor_draw_color(&system->floor, grid_pos, 0, NONE, 0.0f, is_shaded);
                    }
                }
                break;
            }
            case ART_END: {
                gl_exit_draw(&system->exit, time, grid_pos);
                break;
            }
            case ART_EYE:
            case ART_PLAYER: {
                if (is_shaded)
                    break;

                DirectionComponent* dir = get_component(engine, drawable_entity_id, COMP_DIRECTION);
                assert(dir != NULL);

                int controlled = has_component(engine, drawable_entity_id, COMP_INPUTRECEIVER);

                float rotation_angle = DIRECTION_TO_NORTH_FLOAT_ANGLE_RADIANS_2D(dir->dir);
                gl_actor_draw(art->type == ART_EYE ? &system->eye : &system->bot,
                        time, grid_pos, rotation_angle, art->type == ART_PLAYER, controlled, drawable_entity_id);
                break;
            }
            case ART_HEXPRISM:
            case ART_TETRAHEDRON: {
                if (is_shaded)
                    break;
                
                if (art->type == ART_HEXPRISM) {
                    gl_hexprism_draw(&system->hexprism, time, grid_pos);
                } else {
                    gl_tetrahedron_draw(&system->tetrahedron, time, grid_pos);
                }
                break;
            }
            case ART_DOOR: {
                DirectionComponent* dir = get_component(engine, drawable_entity_id, COMP_DIRECTION);
                ActivationComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATION);
                ColorComponent* color_comp = get_component(engine, drawable_entity_id, COMP_COLOR);
                assert(dir != NULL);
                assert(act != NULL);
    
                gl_floor_draw_color(&system->floor, grid_pos, 0, act->active ? ACTIVE : INACTIVE, 0.0f, is_shaded);
                if (!act->active) {
                    if (!is_shaded) {
                        gl_door_draw(&system->door[dir->dir == N || dir->dir == S ? Y : X],
                                     time, grid_pos, color_comp->color);
                    }
                }
                break;
            }
            case ART_BUTTON_HOLD:
            case ART_BUTTON_ONCE: {
//                if (is_shaded)
//                    break;
                
                ColorComponent* color_comp = get_component(engine, drawable_entity_id, COMP_COLOR);
                ActivationComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATION);
                assert(color_comp != NULL);
                assert(act != NULL);
                
                gl_floor_draw_color(&system->floor, grid_pos, 0, act->active ? ACTIVE : INACTIVE, 0.0f, is_shaded);
                if (!is_shaded)
                    gl_button_draw(&system->button, time, grid_pos,
                                   color_comp->color, art->type == ART_BUTTON_ONCE, act->active);
                break;
            }
            case ART_TELEPORT: {
                if (is_shaded)
                    break;
    
                ColorComponent* color_comp = get_component(engine, drawable_entity_id, COMP_COLOR);
                int teleport_id = (int) color_comp->color;
                
                gl_teleport_draw(&system->teleport, time, grid_pos, teleport_id, 1);
                break;
            }
            default: {
                fatal("Unsupported art: %d\n", art->type);
                //ignore unsupported ART
            }
        }
    }
    
    graphics_end_draw(system->graphics);
}
