#ifndef ENGINE_H
#define ENGINE_H

typedef struct Engine Engine;

#include "context.h"
#include "entity.h"
#include "system/action_system.h"
#include "system/activation_system.h"
#include "system/animation_system.h"
#include "system/bot_automaton_system.h"
#include "system/bot_vision_system.h"
#include "system/button_system.h"
#include "system/camera_system.h"
#include "system/container_system.h"
#include "system/endlevel_system.h"
#include "system/gravity_system.h"
#include "system/input_system.h"
#include "system/move_system.h"
#include "system/render_system.h"
#include "system/teleport_system.h"
#ifdef RPI
#include "system/eyeimu_system.h"
#include "system/led_system.h"
#endif
#include <glmc.h>
#include <stdint.h>

#include "es_memory_manager_collections.h"

#include "es_memory_manager.h"
#include "system/visibility_system.h"

typedef struct Engine {
    Context context;

    // systems
    RenderSystem* render_system;
    InputSystem* input_system;
    ButtonSystem* button_system;
    ActionSystem* action_system;
    ActivationSystem* activation_system;
    AnimationSystem* animation_system;
    CameraSystem* camera_system;
    ContainerSystem* container_system;
    EndLevelSystem* end_system;
    MoveSystem* move_system;
    GravitySystem* gravity_system;
    TeleportSystem* teleport_system;
    VisibilitySystem* visibility_system;
    BotVisionSystem* botvision_system;
    BotAutomatonSystem* botautomaton_system;

#ifdef RPI
    LedSystem* led_system;
    EyeImuSystem* eyeimu_system;
#endif

    ESMemory es_memory;
} Engine;

Engine* engine_alloc(Graphics*, LevelName);
void engine_init(Engine*, Graphics*, LevelName);
void engine_free(Engine*);

void engine_update(Engine*);

#endif // ENGINE_H
