#include "teleport_system.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

TeleportSystem* system_teleport_alloc(Engine* engine) {
    TeleportSystem* res = calloc(1, sizeof(TeleportSystem));
    system_teleport_init(res, engine);
    return res;
}

void system_teleport_init(TeleportSystem* system, Engine* engine) {
    system->system_id = TELEPORT_SYSTEM;
}


void system_teleport_free(TeleportSystem* system) {
    
}


void system_teleport_update(TeleportSystem* system, Engine* engine) {
    EntityIterator teleporter_entity;
    search_entity_1(engine, COMP_TELEPORT, &teleporter_entity, system->system_id);
    //for each entity with COMP_TELEPORT
    while(next_entity(&teleporter_entity)){
        EntityId tele_entity_id = teleporter_entity.entity_id;
        TeleportComponent* teleport_component = get_component(engine, tele_entity_id, COMP_TELEPORT);
        LastTeleportInfo* last_teleport_component = get_component(engine, tele_entity_id, COMP_LAST_TELEPORT_INFO);
        GridLocationComponent* tele_location = get_component(engine, tele_entity_id, COMP_GRIDLOCATION);

        EntityIterator on_teleporter;
        search_grid_entity_1(engine, tele_location->pos, 0, COMP_WALKABLE, &on_teleporter, system->system_id);
        //for each entity with COMP_WALKABLE on the position of the teleporter
        while(next_entity(&on_teleporter)){
            EntityId on_tele_id = on_teleporter.entity_id;
            GridLocationComponent* on_tele_location = get_component(engine, on_tele_id, COMP_GRIDLOCATION);
            GridLocationComponent* tele_to_location = get_component(engine, teleport_component->linked_teleporter, COMP_GRIDLOCATION);
            EntityIterator it;
            uint8_t tp_is_empty = 1;
            search_grid_entity_1(engine, tele_to_location->pos, 0, COMP_ART, &it, system->system_id);
            //for each entity with COMP_ART on the position of the corresponding teleporter
            while(next_entity(&it)){
                ArtComponent* art_component = get_component(engine, it.entity_id, COMP_ART);
                //if there is a tetra/hex or bot on the other side, you can't tp to it.
                if(art_component->type == ART_TETRAHEDRON || art_component->type == ART_HEXPRISM || art_component->type == ART_PLAYER){
                    tp_is_empty = 0;
                    break;
                }
            }
            //if the tp on the other side is empty and the entity on the tp, isn't the last teleported one.
            //tp the entity
            if(last_teleport_component->last_teleported != on_tele_id && tp_is_empty){
                //change pos to corresponding tp
                glmc_assign_ivec3(on_tele_location->pos, tele_to_location->pos);
                //change last teleported info
                last_teleport_component->last_teleported = on_tele_id;
                LastTeleportInfo* to_teleport_component = get_component(engine, teleport_component->linked_teleporter, COMP_LAST_TELEPORT_INFO);
                to_teleport_component->last_teleported = on_tele_id;
            }
        }
    }
}
