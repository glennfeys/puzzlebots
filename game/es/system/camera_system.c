#include "camera_system.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

CameraSystem* system_camera_alloc(Engine* engine) {
    CameraSystem* res = calloc(1, sizeof(CameraSystem));
    system_camera_init(res, engine);
    return res;
}

void system_camera_init(CameraSystem* system, Engine* engine) {
    system->system_id = CAMERA_SYSTEM;
}


void system_camera_free(CameraSystem* system) {
    
}

static float move_closer(float cur, float target, float diff_seconds) {
    if (cur == target) return cur;
    float diff = cur - target;
    float partial = diff * (diff_seconds * (CAMERA_PERCENT_SPEED_PER_S/100.0f));
    if (fabsf(partial) < CAMERA_MIN_SPEED_PER_TICK)
        partial = diff < 0.0f ? MAX(diff, -CAMERA_MIN_SPEED_PER_TICK) : MIN(diff, CAMERA_MIN_SPEED_PER_TICK);
    return cur - partial;
}

void system_camera_update(CameraSystem* system, Engine* engine) {
    CameraLookFromComponent* camera_look_from_comp = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
    EntityId camera_look_at_entity_id = search_first_entity_1(engine, COMP_CAMERA_LOOK_AT, system->system_id);

    if (camera_look_at_entity_id == NO_ENTITY || camera_look_from_comp == NULL) {
        printf("No camera\n");
        return;
    }

    CameraLookAtComponent* camera_look_at_comp =
            get_component(engine, camera_look_at_entity_id, COMP_CAMERA_LOOK_AT);
    GridLocationComponent* camera_look_at_grid_comp =
            get_component(engine, camera_look_at_entity_id, COMP_GRIDLOCATION);

    assert(camera_look_from_comp != NULL);
    assert(camera_look_at_entity_id != NO_ENTITY);
    assert(camera_look_at_comp != NULL);
    assert(camera_look_at_grid_comp != NULL);

    if (camera_look_at_grid_comp == NULL)
        return;

    t_vec3 cameraLookAtTargetPos;
    if (has_component(engine, camera_look_at_entity_id, COMP_MOVE_ANIMATION)){
        MoveAnimationComponent* move_animation = get_component(engine, camera_look_at_entity_id, COMP_MOVE_ANIMATION);
        glmc_assign_vec3(cameraLookAtTargetPos, move_animation->curr_pos);
    }
    else{
        glmc_assign_vec3_from_ivec3(cameraLookAtTargetPos, camera_look_at_grid_comp->pos);
    }

    //uncomment to use position of move animation (= smoother camera transition)
//    MoveAnimationComponent* move_anim = get_component(engine, camera_look_at_entity_id, COMP_MOVE_ANIMATION);
//    if (move_anim != NULL && move_anim->position < 1.0f) {
//        glmc_assign_vec3(cameraLookAtTargetPos, move_anim->cur);
//    }

    uint64_t prevUpdate = camera_look_at_comp->last_update;
    uint64_t now = engine->context.time;
    camera_look_at_comp->last_update = now;

    float diffSeconds = prevUpdate == 0 ? 0.0f : (now-prevUpdate) / 1000.0f;

    if (!glmc_equal_vec3(camera_look_at_comp->pos, cameraLookAtTargetPos)) {
        camera_look_at_comp->pos[0] = move_closer(camera_look_at_comp->pos[0], cameraLookAtTargetPos[0], diffSeconds);
        camera_look_at_comp->pos[1] = move_closer(camera_look_at_comp->pos[1], cameraLookAtTargetPos[1], diffSeconds);
        camera_look_at_comp->pos[2] = move_closer(camera_look_at_comp->pos[2], cameraLookAtTargetPos[2], diffSeconds);
    }


    float camRadXY = DEGREES_TO_RADIANS_F(camera_look_from_comp->XYdegrees);
    float camRadZ = DEGREES_TO_RADIANS_F(camera_look_from_comp->Zdegrees);

    float cameraDistanceXY = camera_look_from_comp->distance * sinf(camRadZ);

    camera_look_from_comp->pos[0] = camera_look_at_comp->pos[0] + (sinf(camRadXY) * cameraDistanceXY);
    camera_look_from_comp->pos[1] = camera_look_at_comp->pos[1] - (cosf(camRadXY) * cameraDistanceXY);
    camera_look_from_comp->pos[2] = camera_look_at_comp->pos[2] + (camera_look_from_comp->distance * cosf(camRadZ));
}
