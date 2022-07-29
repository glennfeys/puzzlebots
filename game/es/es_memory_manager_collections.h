#ifndef SYSPROG_ENGINE_COLLECTIONS_H
#define SYSPROG_ENGINE_COLLECTIONS_H

typedef struct ComponentIterator ComponentIterator;
typedef struct EntityList EntityList;
typedef struct EntityIterator EntityIterator;
typedef struct Engine Engine;

#include "entity.h"
#include "component_enums.h"
#include "../../util/benchmarking_utils.h"
#include "system_enums.h"

#include <stdint.h>
#include <glmc.h>

#define NO_ENTITY (0xFFFFFFFF)

typedef struct ComponentIterator {
    Engine* engine;
    EntityId entity_id;
    ComponentId component_id;
    void* comp;
} ComponentIterator;

void search_component(Engine* engine,
                      ComponentId component_id,
                      ComponentIterator* res);
int next_component(ComponentIterator*);

//returns a pointer to the component
void* search_first_component(Engine* engine, ComponentId component_id);

typedef uint64_t component_id_filter_t;

typedef struct EntityIterator {
    Engine* engine;
    EntityId entity_id;
    component_id_filter_t component_id_filter;
    int search_mode; //mode: 0=ANY match  1=also match POS XYZ  2=also match POS XY
    t_ivec3 pos;
} EntityIterator;


component_id_filter_t set_requires_component(component_id_filter_t component_id_filter, ComponentId component_id);

void search_entity(Engine* engine,
                   component_id_filter_t component_id_filter,
                   EntityIterator* res,
                   SystemId system);
int next_entity(EntityIterator*);

void search_entity_1(Engine* engine,
                     ComponentId component_id,
                     EntityIterator* res,
                     SystemId system);
void search_entity_2(Engine* engine,
                     ComponentId component_id1,
                     ComponentId component_id2,
                     EntityIterator* res,
                     SystemId system);
void search_entity_3(Engine* engine,
                     ComponentId component_id1,
                     ComponentId component_id2,
                     ComponentId component_id3,
                     EntityIterator *res,
                     SystemId system);

void search_grid_entity_1(Engine* engine,
                          t_ivec3 pos,
                          int ignore_z,
                          ComponentId component_id1,
                          EntityIterator *res,
                          SystemId system);

void search_grid_entity_2(Engine* engine,
                          t_ivec3 pos,
                          int ignore_z,
                          ComponentId component_id1,
                          ComponentId component_id2,
                          EntityIterator* res,
                          SystemId system);

EntityId search_first_entity_with_mask(Engine *engine, component_id_filter_t component_id_filter, SystemId system);
EntityId search_first_entity_1(Engine *engine, ComponentId component_id, SystemId system);
EntityId search_first_entity_2(Engine *engine, ComponentId component_id, ComponentId component_id2, SystemId system);
EntityId search_first_entity_3(Engine *engine, ComponentId component_id, ComponentId component_id2, ComponentId component_id3,
                               SystemId system);

#endif //SYSPROG_ENGINE_COLLECTIONS_H
