#include "es_memory_manager.h"

#include "../bot_automaton/BotAutomaton_c_wrapper.h"
#include <assert.h>

void es_memory_manager_init(ESMemory* mem) {
    BENCH_TRACE("es_memory_manager_init");
    mem->next_entity_id = 0;

    for (ComponentId component_id = (ComponentId)0; component_id < COMPONENT_ID_SIZE;
         component_id++) {
        for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) {
            memset(&mem->components[component_id][entity_id], 0, sizeof(AllComponent));
            mem->components[component_id][entity_id].free = 1;
        }
    }
}

void es_memory_manager_free(ESMemory* mem) {
    BENCH_TRACE("es_memory_manager_free");
    for (int i = 0; i < sizeof(mem->components[COMP_BOT_AUTOMATON_2]) /
                            sizeof(mem->components[COMP_BOT_AUTOMATON_1][0]);
         i++) {
        freeBotBerri(mem->components[COMP_BOT_AUTOMATON_2][i].bot_automation2_component.botberri);
    }
    // free any component that needs freeing. (most don't)
}

int has_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    BENCH_TRACE("has_component", entity_id, component_id);
    fatal_if(entity_id == NO_ENTITY, "has_component(engine, entity_id==NO_ENTITY, component_id=%d)",
             component_id);
    assert(component_id < COMPONENT_ID_SIZE);
    assert(component_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(entity_id >= 0);
    return !engine->es_memory.components[component_id][entity_id].free;
}

void* get_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    BENCH_TRACE("get_component", entity_id, component_id);
    fatal_if(entity_id == NO_ENTITY, "get_component(engine, entity_id==NO_ENTITY, component_id=%d)",
             component_id);
    assert(component_id < COMPONENT_ID_SIZE);
    assert(component_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(entity_id >= 0);
    if (engine->es_memory.components[component_id][entity_id].free)
        return NULL;
    assert(!engine->es_memory.components[component_id][entity_id].free);
    return &engine->es_memory.components[component_id][entity_id].camera_lookfrom;
}

void* create_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    BENCH_TRACE("create_component", entity_id, component_id);
    fatal_if(entity_id == NO_ENTITY,
             "create_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
    assert(entity_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(component_id >= 0);
    assert(component_id < COMPONENT_ID_SIZE);
    assert(engine->es_memory.components[component_id][entity_id].free);
    engine->es_memory.components[component_id][entity_id].free = 0;
    return &engine->es_memory.components[component_id][entity_id].camera_lookfrom;
}

void free_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    BENCH_TRACE("free_component", entity_id, component_id);
    fatal_if(entity_id == NO_ENTITY,
             "free_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
    assert(entity_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(component_id >= 0);
    assert(component_id < COMPONENT_ID_SIZE);

    assert(!engine->es_memory.components[component_id][entity_id].free);
    engine->es_memory.components[component_id][entity_id].free = 1;

    // if any components need cleanup, do it here
    // TODO

    if (component_id == COMP_BOT_AUTOMATON_2) {
        freeBotBerri(engine->es_memory.components[component_id][entity_id]
                         .bot_automation2_component.botberri);
    }
}

EntityId get_new_entity_id(Engine* engine) {
    BENCH_TRACE("get_new_entity_id");
    if (engine->es_memory.next_entity_id == MAX_ENTITIES) {
        fatal("Fatal error: Maximum number of entities used: %u", MAX_ENTITIES);
    }
    return engine->es_memory.next_entity_id++;
}

void delete_entity(Engine* engine, EntityId entity_id) {
    for (ComponentId component_id = (ComponentId)0; component_id < COMPONENT_ID_SIZE;
         component_id++) {
        if (!engine->es_memory.components[component_id][entity_id].free) {
            free_component(engine, entity_id, component_id);
        }
    }
}

void register_system_component(ESMemory* mem, SystemId system_type, uint8_t component_count,
                               const ComponentId components[]) {
    /*
     * This method is never called in the given memory manager but will be needed when improving the
     * memory system. The code that is given here will be needed to create you own benchmark file.
     *
     * The register_system_component is present in the given memory manager so that it will be able
     * to replay benchmark files created using the optimized memory manager.
     */
    assert(component_count <= 6);
#ifdef BENCHMARK
    ComponentId comps[6] = {0};
    for (int i = 0; i < component_count; i++) {
        comps[i] = components[i];
    }
    // This method to pass the register_system_component arguments means that there is a maximum of
    // 6 components per system !
    BENCH_TRACE("register_system_component", system_type, component_count, comps[0], comps[1],
                comps[2], comps[3], comps[4], comps[5]);
#endif

    // TODO
}
