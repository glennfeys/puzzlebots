#ifndef SYSPROG_COMPONENTS_H
#define SYSPROG_COMPONENTS_H

#define EMPTY_COMPONENT_STRUCT char c
#define TO_IMPLEMENT_STRUCT char c

#include "component_enums.h"
#include "entity.h"

#include "../fsm/fsm.h"
#include "../graphics/opengl_game_renderer.h"

#include "../bot_automaton/BotAutomaton_c_wrapper.h"
#include "game_util.h"

#include <glmc.h>
#include <stdint.h>

typedef struct CameraLookFromComponent {
    t_vec3 pos;

    float distance;
    float XYdegrees;
    float Zdegrees;
} CameraLookFromComponent;

typedef struct CameraLookAtComponent {
    t_vec3 pos;

    uint64_t last_update;
} CameraLookAtComponent;

typedef struct MoveActionComponent {
    uint8_t is_forward;
} MoveActionComponent;

typedef struct GridLocationComponent {
    t_ivec3 pos;
} GridLocationComponent;

typedef struct OneTimeAnimationComponent {
    t_ivec3 destination_pos;
    float speed; // lower is better (average +/- 500)
    double animation_pos;
    double prev_time;
} OneTimeAnimationComponent;

typedef struct MoveAnimationComponent {
    t_vec3 curr_pos;
    int is_movable;
} MoveAnimationComponent;

typedef struct GravityComponent {
    // TODO
    TO_IMPLEMENT_STRUCT;
} GravityComponent;

typedef struct WalkComponent {
    int walkHeight; // 0 or 1
} WalkComponent;

typedef struct WallArtComponent {
    int has_floor;
    int wall_height[4]; // indexed by Direction, number of walls stacked on the floor
} WallArtComponent;

typedef struct BlockingComponent {
    // TODO
    TO_IMPLEMENT_STRUCT;
} BlockingComponent;

typedef struct ItemComponent {
    // TODO
    TO_IMPLEMENT_STRUCT;
} ItemComponent;

typedef struct InContainerComponent {
    EntityId container;
} InContainerComponent;

typedef struct ContainerComponent {
    EntityId item;
} ContainerComponent;

typedef struct PushableComponent {
    uint8_t push_gen; // the number of blocks pushed
} PushableComponent;

typedef struct ButtonComponent {
    int requires_hold;
} ButtonComponent;

typedef struct TeleportComponent {
    EntityId linked_teleporter;
} TeleportComponent;

typedef struct LastTeleportInfo {
    EntityId last_teleported;
} LastTeleportInfo;

typedef struct ButtonActivatorComponent {
    EMPTY_COMPONENT_STRUCT;
} ButtonActivatorComponent;

typedef struct ConditionalActivationComponent {
    int activation_id;
} ConditionalActivationComponent;

typedef struct ActivationComponent {
    int activation_id;
    int active;
} ActivationComponent;

typedef struct ColorComponent {
    GameColor color;
} ColorComponent;

typedef struct InputReceiverComponent {
    EMPTY_COMPONENT_STRUCT;
} InputReceiverComponent;

typedef struct ExitAffectedComponent {
    int required_for_level_finish;
} ExitAffectedComponent;

typedef struct ControllableComponent {
    EMPTY_COMPONENT_STRUCT;
} ControllableComponent;

typedef struct ItemActionComponent {
    int do_action;
} ItemActionComponent;

typedef struct DirectionComponent {
    Direction dir;
} DirectionComponent;

typedef struct ArtComponent {
    ArtType type;
} ArtComponent;

typedef struct ExitComponent {
    EMPTY_COMPONENT_STRUCT;
} ExitComponent;

typedef struct ExitedComponent {
    EMPTY_COMPONENT_STRUCT;
} ExitedComponent;

typedef struct VisionComponent {
    EMPTY_COMPONENT_STRUCT;
} VisionComponent;

typedef struct VisibleComponent {
    uint8_t is_shaded;
} VisibleComponent;

typedef struct BotAutomaton1Component {
    uint8_t is_active;
    char state[20];
} BotAutomaton1Component;

typedef struct BotAutomaton2Component {
    uint8_t is_active;
    BotBerri* botberri;
} BotAutomaton2Component;

typedef struct BotVisionComponent {
    uint32_t vision_mask;
    t_ivec3 locations[4];
} BotVisionComponent;

typedef struct DelayedActionComponent {
    uint8_t amount;
    uint8_t actions[MAX_FSM_ACTION];
} DelayedActionComponent;

#endif // SYSPROG_COMPONENTS_H
