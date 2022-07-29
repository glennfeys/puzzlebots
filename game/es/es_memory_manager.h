#ifndef SYSPROG_ES_MEMORY_MANAGER_H
#define SYSPROG_ES_MEMORY_MANAGER_H

#include "entity.h"
#define MAX_ENTITIES 500u

#include "component_structs.h"
#include "../../util/benchmarking_utils.h"
#include <assert.h>

static_assert(COMPONENT_ID_SIZE >= 35, "There must be a minimum number of components to be able to replay the provided benchmark");

typedef struct AllComponent {
    int free;
    
    union {
        CameraLookFromComponent camera_lookfrom;
        CameraLookAtComponent camera_lookat;
        MoveActionComponent move_action;
        GridLocationComponent gridlocation_component;
        OneTimeAnimationComponent onetimeanimation_component;
        MoveAnimationComponent moveanimation_component;
        WallArtComponent wallinfo_component;
        WalkComponent walk_component;
        BlockingComponent blocking_component;
        ItemComponent item_component;
        InContainerComponent incontainer_component;
        ContainerComponent container_component;
        ActivationComponent activation_component;
        InputReceiverComponent inputreceiver_component;
        ExitAffectedComponent exit_require_component;
        ControllableComponent controllable_component;
        MoveActionComponent moveaction_component;
        ItemActionComponent itemaction_component;
        DirectionComponent direction_component;
        ArtComponent art_component;
        ExitComponent exit_component;
        ExitedComponent exited_component;
        VisionComponent vision_component;
        VisibleComponent visible_component;
        ButtonComponent button_component;
        ButtonActivatorComponent button_activator_component;
        PushableComponent pushable_component;
        ColorComponent color_component;
        TeleportComponent teleport_component;
        LastTeleportInfo lastteleportinfo_component;
        ConditionalActivationComponent conditional_activation_component;
        BotVisionComponent bot_sense_component;
        BotAutomaton1Component bot_automation1_component;
        BotAutomaton2Component bot_automation2_component;
    };
} AllComponent;

typedef struct ESMemory {
    EntityId next_entity_id;
    
    AllComponent components[COMPONENT_ID_SIZE][MAX_ENTITIES];
} ESMemory;

void es_memory_manager_init(ESMemory*);
void es_memory_manager_free(ESMemory*);

#include "engine.h"

void register_system_component(ESMemory* memory, SystemId system, uint8_t component_count, const ComponentId[]);

int has_component(Engine* engine, EntityId entity_id, ComponentId component_id);
void* get_component(Engine* engine, EntityId entity_id, ComponentId component_id);

void* create_component(Engine* engine, EntityId entity_id, ComponentId component_id);
void free_component(Engine* engine, EntityId entity_id, ComponentId component_id);

EntityId get_new_entity_id(Engine* engine);
void delete_entity(Engine* engine, EntityId entity_id);

#endif //SYSPROG_ES_MEMORY_MANAGER_H
