#include "visibility_system.h"
#include "../assemblage.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

VisibilitySystem* system_visibility_alloc(Engine* engine) {
    VisibilitySystem* res = calloc(1, sizeof(VisibilitySystem));
    system_visibility_init(res, engine);
    return res;
}

void system_visibility_init(VisibilitySystem* system, Engine* engine) {
    system->system_id = VISIBILITY_SYSTEM;
}


void system_visibility_free(VisibilitySystem* system) {
    
}

void system_visibility_update(VisibilitySystem* system, Engine* engine) {

    EntityIterator visible_it;
    search_entity_1(engine, COMP_VISIBLE, &visible_it, system->system_id);
    //make all visisble tiles grey
    while(next_entity(&visible_it)){
        EntityId visible_id = visible_it.entity_id;
        VisibleComponent* visible_comp = get_component(engine, visible_id, COMP_VISIBLE);
        //if a visible tile isn't shaded, make it shaded.
        if(! visible_comp->is_shaded){
            visible_comp->is_shaded = 1;
        }
    }
    EntityIterator vision_it;
    search_entity_1(engine, COMP_VISION, &vision_it, system->system_id);
    //search which tiles need to be visible with color
    while(next_entity(&vision_it)){
        EntityId vision_id = vision_it.entity_id;
        BotVisionComponent* bot_vision_comp = get_component(engine, vision_id, COMP_BOT_VISION);
        //for each position we have to check.
        for(int i=0; i<sizeof(bot_vision_comp->locations)/sizeof(bot_vision_comp->locations[0]); i++){
            t_ivec3 location;
            glmc_assign_ivec3(location, bot_vision_comp->locations[i]);
            EntityIterator visible_item_it;
            search_grid_entity_1(engine, location, 1, COMP_GRIDLOCATION, &visible_item_it, system->system_id);
            //make all the entities with gridlocation visible with color on location
            while(next_entity(&visible_item_it)){
                EntityId visible_item_id = visible_item_it.entity_id;
                //give all items COMP_VISIBLE
                if(!has_component(engine, visible_item_id, COMP_VISIBLE)){
                    create_component(engine, visible_item_id, COMP_VISIBLE);
                }
                VisibleComponent* item_visible_comp = get_component(engine, visible_item_id, COMP_VISIBLE);
                //make visible with color
                item_visible_comp->is_shaded = 0;
            }
        } 
    }
}
