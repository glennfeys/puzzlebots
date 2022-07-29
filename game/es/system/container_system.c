#include "container_system.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

ContainerSystem* system_container_alloc(Engine* engine) {
    ContainerSystem* res = calloc(1, sizeof(ContainerSystem));
    system_container_init(res, engine);
    return res;
}

void system_container_init(ContainerSystem* system, Engine* engine) {
    system->system_id = CONTAINER_SYSTEM;
}

void system_container_free(ContainerSystem* system) {}

void system_container_update(ContainerSystem* system, Engine* engine) {
    EntityIterator item_it;
    search_entity_1(engine, COMP_INCONTAINER, &item_it, system->system_id);
    //for each entity with COMP_INCONTAINER
    while (next_entity(&item_it)) {
        EntityId item_eid = item_it.entity_id;
        InContainerComponent* incontainer_comp = get_component(engine, item_eid, COMP_INCONTAINER);
        EntityId container_eid = incontainer_comp->container;
        MoveAnimationComponent* itemAnim = get_component(engine, item_eid, COMP_MOVE_ANIMATION);
        MoveAnimationComponent* botAnim = get_component(engine, container_eid, COMP_MOVE_ANIMATION);
        //if there is no animation going on
        if (botAnim != NULL) {
            //if there is no item animation going on, create one
            if (itemAnim == NULL) {
                itemAnim = create_component(engine, item_eid, COMP_MOVE_ANIMATION);
            }
            //the item position is 1 above the pos of the bot
            t_vec3 itempos;
            glmc_assign_vec3(itempos, botAnim->curr_pos);
            itempos[ZZ] += 1;
            if (!glmc_equal_vec3(itemAnim->curr_pos, itempos)) {
                glmc_assign_vec3(itemAnim->curr_pos, itempos);
            }
        } else { // if there is an animation going on
            GridLocationComponent* itemLoc = get_component(engine, item_eid, COMP_GRIDLOCATION);
            GridLocationComponent* botLoc = get_component(engine, container_eid, COMP_GRIDLOCATION);
            //the item position is 1 above the pos of the bot
            t_ivec3 itempos;
            glmc_assign_ivec3(itempos, botLoc->pos);
            itempos[ZZ] += 1;
            //if the grid location of the item is different from the on above the bot
            if (!glmc_equal_ivec3(itemLoc->pos, itempos)) {
                //free the item animation if it has one
                if (has_component(engine, item_eid, COMP_MOVE_ANIMATION)) {
                    free_component(engine, item_eid, COMP_MOVE_ANIMATION);
                }
                //set the position of the item one above the bot
                glmc_assign_ivec3(itemLoc->pos, itempos);
            }
        }
    }
}
