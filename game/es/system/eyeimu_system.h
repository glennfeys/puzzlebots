#ifndef EYEIMU_SYSTEM_H
#define EYEIMU_SYSTEM_H

#include <stdint.h>

#include "../system_enums.h"
#include "../../imu/RTIMU_c_wrapper.h"

typedef struct EyeImuSystem {
    SystemId system_id;
    IMUSettings* settings;
    IMU* imu;
    uint64_t move_timer;
    float threshold;
} EyeImuSystem;

#include "../engine.h"

void system_eyeimu_init(EyeImuSystem*, Engine*);
EyeImuSystem* system_eyeimu_alloc(Engine*);
void system_eyeimu_update(EyeImuSystem*, Engine*);
void system_eyeimu_free(EyeImuSystem*);

#endif // EYEIMU_SYSTEM_H
