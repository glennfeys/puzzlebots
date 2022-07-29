#include "action_system.h"
#include <assert.h>
#include <stdlib.h>

ActionSystem* system_action_alloc(Engine* engine) {
    ActionSystem* res = calloc(1, sizeof(ActionSystem));
    system_action_init(res, engine);
    return res;
}

void system_action_init(ActionSystem* system, Engine* engine) { system->systemId = ACTION_SYSTEM; }

void system_action_free(ActionSystem* system) {}

void system_action_update(ActionSystem* system, Engine* engine) {
    EntityIterator itemaction_it;

    search_entity_2(engine, COMP_ITEMACTION, COMP_CONTAINER, &itemaction_it, system->systemId);
    //for each entity with COMP_ITEMACTION and  COMP_CONTAINER
    while (next_entity(&itemaction_it)) {
        EntityId itemaction_eid = itemaction_it.entity_id;

        GridLocationComponent* itemactiongl_comp =
            get_component(engine, itemaction_eid, COMP_GRIDLOCATION);
        ItemActionComponent* itemaction_comp =
            get_component(engine, itemaction_eid, COMP_ITEMACTION);
        ContainerComponent* container_comp = get_component(engine, itemaction_eid, COMP_CONTAINER);

        EntityIterator item_it;
        search_grid_entity_1(engine, itemactiongl_comp->pos, 0, COMP_ITEM, &item_it,
                             system->systemId);
        //if do_action is active
        if (itemaction_comp->do_action) {
            // pick up item if it does not have a container yet
            if (container_comp->item == NO_ENTITY) {
                if (next_entity(&item_it)) {
                    EntityId item_eid = item_it.entity_id;
                    // avoids that bot can pick up an item that is already in another bot's
                    // container
                    if (!has_component(engine, item_eid, COMP_INCONTAINER)) {
                        InContainerComponent* incontainer_comp =
                            create_component(engine, item_eid, COMP_INCONTAINER);
                        incontainer_comp->container = itemaction_eid;
                        container_comp->item = item_eid;
                    }
                }
            // drop item if there is no other item on the tile yet
            } else if (!next_entity(&item_it)) {
                EntityId item_eid = container_comp->item;
                GridLocationComponent* itemgl_comp =
                    get_component(engine, item_eid, COMP_GRIDLOCATION);
                glmc_assign_ivec3(itemgl_comp->pos, itemactiongl_comp->pos);
                free_component(engine, item_eid, COMP_INCONTAINER);
                container_comp->item = NO_ENTITY;
            }
        }
        itemaction_comp->do_action = 0;
    }
}
