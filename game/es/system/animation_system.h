#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "../system_enums.h"

typedef struct AnimationSystem {
	SystemId systemId;
} AnimationSystem;

#include "../engine.h"

void system_animation_init(AnimationSystem*, Engine*);
AnimationSystem* system_animation_alloc(Engine*);
void system_animation_update(AnimationSystem*, Engine*);
void system_animation_free(AnimationSystem*);

void add_bot_exit_animation(Engine*, EntityId exiting_entity_id, EntityId exit_entity_id);


#endif