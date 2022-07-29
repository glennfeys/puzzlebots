#include "animation_system.h"
#include <stdlib.h>
#include <assert.h>

AnimationSystem* system_animation_alloc(Engine* engine) {
    AnimationSystem* res = calloc(1, sizeof(AnimationSystem));
    system_animation_init(res, engine);
    return res;
}

void system_animation_init(AnimationSystem* system, Engine* engine) {
    system->systemId = ANIMATION_SYSTEM;
}


void system_animation_free(AnimationSystem* system) {
}

void system_animation_update(AnimationSystem* system, Engine* engine) {
    EntityIterator it;

    search_entity_2(engine, COMP_MOVE_ANIMATION, COMP_ONE_TIME_ANIMATION, &it, system->systemId);
    //for each entity with COMP_MOVE_ANIMATION and COMP_ONE_TIME_ANIMATION
    while(next_entity(&it)) {
        EntityId moving_entity_id = it.entity_id;

        OneTimeAnimationComponent* onetime_animation = get_component(engine, moving_entity_id, COMP_ONE_TIME_ANIMATION); 
        MoveAnimationComponent* move_animation = get_component(engine, moving_entity_id, COMP_MOVE_ANIMATION);

        //current time / speed
        float time = engine->context.time / onetime_animation->speed;

        //time between this tick and previous tick
        float delta_time = time - onetime_animation->prev_time;
        onetime_animation->prev_time = time;

        //movement the entity has to do
        t_ivec3 vector;
        glmc_assign_ivec3(vector, onetime_animation->destination_pos);
        
        GridLocationComponent* grid_location = get_component(engine, moving_entity_id, COMP_GRIDLOCATION);
        //if the animation begins, set the current position to the grid_location of the entity
        if (onetime_animation->animation_pos == 0) {
            glmc_assign_vec3_from_ivec3(move_animation->curr_pos, grid_location->pos);
        }
        //every tick move the entity with a fraction
        if (onetime_animation->animation_pos < 1) {
            onetime_animation->animation_pos += delta_time;
            move_animation->curr_pos[XX] += vector[XX] * delta_time;
            move_animation->curr_pos[YY] += vector[YY] * delta_time;
            move_animation->curr_pos[ZZ] += vector[ZZ] * delta_time;
        }
        //if the animation is done, set the grid location of the entity to the current_position
        //and remove the animation components. 
        else if (onetime_animation->animation_pos >= 1.0) {
            glmc_assign_ivec3_from_vec3(grid_location->pos, move_animation->curr_pos);
            free_component(engine, moving_entity_id, COMP_ONE_TIME_ANIMATION);
            free_component(engine, moving_entity_id, COMP_MOVE_ANIMATION);
        }
    }

}
