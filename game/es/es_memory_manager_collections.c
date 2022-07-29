#include "es_memory_manager_collections.h"
#include "es_memory_manager.h"
#include "engine.h"

#include <stdlib.h>
#include <assert.h>

void search_component(Engine* engine,
                      ComponentId component_id,
                      ComponentIterator* res) {
    res->engine = engine;
    res->component_id = component_id;
    res->entity_id = (EntityId) -1; //bit of a hack
}

int next_component(ComponentIterator* res) {
    for (EntityId entity_id = res->entity_id+1; entity_id < res->engine->es_memory.next_entity_id; entity_id++) {
        if (!res->engine->es_memory.components[res->component_id][entity_id].free) {
            res->entity_id = entity_id;
            res->comp = &res->engine->es_memory.components[res->component_id][entity_id].camera_lookfrom;
            return 1;
        }
    }
    return 0;
}

void* search_first_component(Engine* engine, ComponentId component_id) {
    ComponentIterator it;
    search_component(engine, component_id, &it);
    if (next_component(&it))
        return it.comp;
    else
        return NULL;
}



static int get_requires_component(component_id_filter_t component_id_filter, ComponentId component_id) {
    component_id_filter_t mask = ((component_id_filter_t)1) << component_id;
    return (mask & component_id_filter) != 0;
}
component_id_filter_t set_requires_component(component_id_filter_t component_id_filter, ComponentId component_id) {
    component_id_filter_t mask = ((component_id_filter_t)1) << component_id;
    return component_id_filter | mask;
}


void search_entity(Engine *engine, component_id_filter_t component_id_filter, EntityIterator *res, SystemId system) {
    res->engine = engine;
    res->component_id_filter = component_id_filter;
    res->entity_id = (EntityId) -1; //bit of a hack
    res ->search_mode = 0;
    res->pos[0] = 0;
    res->pos[1] = 0;
    res->pos[2] = 0;

    static_assert(COMPONENT_ID_SIZE <= sizeof(component_id_filter_t) * 8,
            "component_id_filter_t should have enough bits to hold all components");
}

void search_entity_1(Engine *engine, ComponentId component_id1, EntityIterator *res, SystemId system) {
    BENCH_TRACE("search_entity_1", component_id1, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    search_entity(engine, component_id_filter, res, system);
}
void search_entity_2(Engine *engine, ComponentId component_id1, ComponentId component_id2, EntityIterator *res,
                     SystemId system) {
    BENCH_TRACE("search_entity_2", component_id1, component_id2, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    search_entity(engine, component_id_filter, res, system);
}
void search_entity_3(Engine *engine, ComponentId component_id1, ComponentId component_id2, ComponentId component_id3,
                     EntityIterator *res, SystemId system) {
    BENCH_TRACE("search_entity_3", component_id1, component_id2, component_id3, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, component_id3);
    search_entity(engine, component_id_filter, res, system);
}

void search_grid_entity_1(Engine *engine, t_ivec3 pos, int ignore_z, ComponentId component_id1, EntityIterator *res,
                          SystemId system) {
    BENCH_TRACE("search_grid_entity_1", pos[0], pos[1], pos[2], ignore_z, component_id1, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, COMP_GRIDLOCATION);
    search_entity(engine, component_id_filter, res, system);
    res->search_mode = ignore_z ? 2 : 1;
    glmc_assign_ivec3(res->pos, pos);
}

void search_grid_entity_2(Engine *engine, t_ivec3 pos, int ignore_z, ComponentId component_id1, ComponentId component_id2,
                          EntityIterator *res, SystemId system) {
    BENCH_TRACE("search_grid_entity_2", pos[0], pos[1], pos[2], ignore_z, component_id1, component_id2, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, COMP_GRIDLOCATION);
    search_entity(engine, component_id_filter, res, system);
    res->search_mode = ignore_z ? 2 : 1;
    glmc_assign_ivec3(res->pos, pos);
}

EntityId search_first_entity_with_mask(Engine *engine, component_id_filter_t component_id_filter, SystemId system) {
    // Dummy TRACE, used to skip to the next intruction in the benchmark (which will always be next_entity).
    BENCH_TRACE("search_first_entity_with_mask");
    EntityIterator it;
    search_entity(engine, component_id_filter, &it, system);
    if (next_entity(&it))
        return it.entity_id;
    else
        return NO_ENTITY;
}
EntityId search_first_entity_1(Engine *engine, ComponentId component_id1, SystemId system) {
    BENCH_TRACE("search_first_entity_1", component_id1, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    return search_first_entity_with_mask(engine, component_id_filter, system);
}
EntityId search_first_entity_2(Engine *engine, ComponentId component_id1, ComponentId component_id2, SystemId system) {
    BENCH_TRACE("search_first_entity_2", component_id1, component_id2, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    return search_first_entity_with_mask(engine, component_id_filter, system);
}
EntityId search_first_entity_3(Engine *engine, ComponentId component_id1, ComponentId component_id2, ComponentId component_id3,
                               SystemId system) {
    BENCH_TRACE("search_first_entity_3", component_id1, component_id2, component_id3, system);
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, component_id3);
    return search_first_entity_with_mask(engine, component_id_filter, system);
}

int next_entity(EntityIterator* res) {
    BENCH_TRACE("next_entity");
    for (EntityId entity_id = res->entity_id+1; entity_id < res->engine->es_memory.next_entity_id; entity_id++) {
        if (res->component_id_filter != 0) {
            for (ComponentId component_id = 0; component_id < COMPONENT_ID_SIZE; component_id++) {
                if (get_requires_component(res->component_id_filter, component_id) &&
                    res->engine->es_memory.components[component_id][entity_id].free) {
                    //no match. Try the next entity
                    goto next_entity_loop;
                }
            }
        }
        if (res->search_mode > 0 && res->search_mode <= 2) {
            if (res->engine->es_memory.components[COMP_GRIDLOCATION][entity_id].free) {
                //no gridlocation
                goto next_entity_loop;
            }
            GridLocationComponent* grid_comp = &res->engine->es_memory.components[COMP_GRIDLOCATION][entity_id].gridlocation_component;
            if (grid_comp->pos[0] != res->pos[0])
                goto next_entity_loop;
            if (grid_comp->pos[1] != res->pos[1])
                goto next_entity_loop;
            if (res->search_mode == 1 && grid_comp->pos[2] != res->pos[2])
                goto next_entity_loop;
        }
        //all match, return it
        res->entity_id = entity_id;
        return 1;
        
        next_entity_loop: ;
    }
    return 0;
}
