#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "../system_enums.h"

#define CAMERA_PERCENT_SPEED_PER_S 200.0f
#define CAMERA_MIN_SPEED_PER_TICK 0.05f

typedef struct CameraSystem {
	SystemId system_id;
} CameraSystem;

#include "../engine.h"

void system_camera_init(CameraSystem*, Engine*);
CameraSystem* system_camera_alloc(Engine*);
void system_camera_update(CameraSystem*, Engine*);
void system_camera_free(CameraSystem*);

#endif //CAMERA_SYSTEM_H