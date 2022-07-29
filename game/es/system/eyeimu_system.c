#include "eyeimu_system.h"

EyeImuSystem* system_eyeimu_alloc(Engine* engine) {
    EyeImuSystem* res = calloc(1, sizeof(EyeImuSystem));
    system_eyeimu_init(res, engine);
    return res;
}

void system_eyeimu_init(EyeImuSystem* system, Engine* engine) {
    system->system_id = EYEIMU_SYSTEM;

    IMUSettings* settings = createIMUSettings();
    IMU* imu = createIMU(settings);

    system->settings = settings;
    system->imu = imu;
    system->move_timer = 0;
    system->threshold = 0.35;

    initIMU(imu);

    setSlerp(imu, 0.02);
    enableGyro(imu, 1);
    enableAccel(imu, 1);
    enableCompass(imu, 1);
}

void system_eyeimu_free(EyeImuSystem* system) {
    freeIMUSettings(system->settings);
    freeIMU(system->imu);
}

void system_eyeimu_update(EyeImuSystem* system, Engine* engine) {
    EntityIterator art_it;
    search_entity_2(engine, COMP_ART, COMP_CONTROLLABLE, &art_it, system->system_id);

    while (next_entity(&art_it)) {
        EntityId art_eid = art_it.entity_id;
        ArtComponent* art_comp = get_component(engine, art_eid, COMP_ART);

        if (art_comp->type == ART_EYE) {
            if (IMURead(system->imu)) {
                uint64_t now = currentSinceEpoch();
                if ((now - system->move_timer) > 1000000) {                  
                    IMUData* data = getData(system->imu);
                    float x = data->x;
                    float y = data->y;
                    float abs_x = x > 0 ? x : (-x);
                    float abs_y = y > 0 ? y : (-y);
                    DirectionComponent* dir_comp = get_component(engine, art_eid, COMP_DIRECTION);
                    
                    t_ivec3 direction;
                    int has_to_move = 0;
                    if (abs_x > abs_y && abs_x > system->threshold) {
                        if (x > 0) {
                            dir_comp->dir = E;
                            glmc_ivec3_set(direction, 1, 0, 0);
                        } else {
                            dir_comp->dir = W;
                            glmc_ivec3_set(direction, -1, 0, 0);
                        }
                        has_to_move = 1;
                    } else if (abs_y > abs_x && abs_y > system->threshold) {
                        if (y > 0) {
                            dir_comp->dir = N;
                            glmc_ivec3_set(direction, 0, 1, 0);
                        } else {
                            dir_comp->dir = S;
                            glmc_ivec3_set(direction, 0, -1, 0);
                        }
                        has_to_move = 1;
                    }

                    if (has_to_move) {
                        GridLocationComponent* eye_gl_comp = get_component(engine, art_eid, COMP_GRIDLOCATION);
                        create_component(engine, art_eid, COMP_MOVE_ANIMATION);
                        create_component(engine, art_eid, COMP_ONE_TIME_ANIMATION);
                        OneTimeAnimationComponent* onetime_animation = get_component(engine, art_eid, COMP_ONE_TIME_ANIMATION);
                    
                        glmc_assign_ivec3(onetime_animation->destination_pos, direction);
                        onetime_animation->animation_pos = 0;
                        onetime_animation->speed = 500.0;
                        onetime_animation->prev_time = engine->context.time / onetime_animation->speed;
                    }

                    system->move_timer = now;
                    free(data);
                }
            }
        }
    }
}
