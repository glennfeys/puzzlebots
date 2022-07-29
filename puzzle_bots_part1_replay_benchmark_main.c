#include "game/es/es_memory_manager.h"
#include "game/es/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#undef main // Don't let SDL overwrite main definition
#ifdef _WIN32
#  ifdef _WIN64
#    define PRI_SIZET PRIu64
#  else
#    define PRI_SIZET PRIu32
#  endif
#else
#  define PRI_SIZET "zu"
#endif

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

void executeCommand(Engine* engine, EntityIterator* res, BenchReading* reading);
int getReading(BenchReading* reading, FILE* file);
void iteratorInit(Engine* engine, component_id_filter_t filter, SystemId system,
                  EntityIterator* res);

int main(int argc, char** argv) {
    char name[100] = "./benchmarks/";
    if (argc == 1) {
        printf("Error: please enter a filename.");
        return (-1);
    }
    strcat(name, argv[1]);
    FILE* file = fopen(name, "rb");
    BenchReading* reading = malloc(sizeof(BenchReading));

    if (file == NULL) {
        printf("Error file doesn't exist\n");
        return (-1);
    }
    clock_t startClock = clock();
    Engine* engine = malloc(sizeof(Engine));
    ESMemory* testMemory = malloc(sizeof(ESMemory));
    engine->es_memory = *testMemory;
    EntityIterator* res = malloc(sizeof(EntityIterator));

    while (getReading(reading, file)) {
        // printf("Function: %s, Arguments: %d, No Args: %d\n", &reading->function_name,
        //        &reading->arguments, &reading->no_args);
        executeCommand(engine, res, reading);
    }
    clock_t benchTime = clock() - startClock;
    float benchTimeSec = (float)benchTime / CLOCKS_PER_SEC;
    uint32_t benchTimeMS = benchTime / CLOCKS_PER_MS;
    printf("Duration: %f seconds or %d milliseconds.\n", benchTimeSec, benchTimeMS);
    printf("Memory usage: %" PRI_SIZET  " bytes, or %f kilobytes ", sizeof(engine->es_memory),
           sizeof(engine->es_memory) / 1000.0);
    free(engine);
    free(testMemory);
    free(reading);
    free(res);
    fclose(file);
    return 0;
}
int getReading(BenchReading* reading, FILE* file) {

    if (fread(&(reading->function_name), sizeof(reading->function_name), 1, file) != 1) {
        return 0;
    }
    if (fread(&(reading->arguments), sizeof(uint64_t), 8, file) != 8) {
        return 0;
    }
    if (fread(&(reading->no_args), sizeof(reading->no_args), 1, file) != 1) {
        return 0;
    }
    return 1;
}

void executeCommand(Engine* engine, EntityIterator* res, BenchReading* reading) {
    char* command = reading->function_name;
    uint64_t* args = reading->arguments;
    if (strcmp(command, "register_system_component") == 0) {
        ComponentId comps[args[1]];
        for (int i = 0; i < args[1]; i++) {
            comps[i] = args[i + 2];
        }
        register_system_component(&(engine->es_memory), args[0], args[1], comps);
    } else if (strcmp(command, "has_component") == 0) {
        has_component(engine, args[0], args[1]);
    } else if (strcmp(command, "get_component") == 0) {
        get_component(engine, args[0], args[1]);
    } else if (strcmp(command, "create_component") == 0) {
        create_component(engine, args[0], args[1]);
    } else if (strcmp(command, "free_component") == 0) {
        free_component(engine, args[0], args[1]);
    } else if (strcmp(command, "get_new_entity_id") == 0) {
        get_new_entity_id(engine);
    } else if (strcmp(command, "delete_entity") == 0) {
        delete_entity(engine, args[0]);
    } else if (strcmp(command, "create_component") == 0) {
        create_component(engine, args[0], args[1]);
    } else if (strcmp(command, "search_first_component") == 0) {
        search_first_component(engine, args[1]);
    } else if (strcmp(command, "set_requires_component") == 0) {
        set_requires_component(args[0], args[1]);
    } else if (strcmp(command, "next_entity") == 0) {
        next_entity(res);
    } else if (strcmp(command, "search_entity_1") == 0) {
        search_entity_1(engine, args[0], res, args[1]);
    } else if (strcmp(command, "search_entity_2") == 0) {
        search_entity_2(engine, args[0], args[1], res, args[2]);
    } else if (strcmp(command, "search_entity_3") == 0) {
        search_entity_3(engine, args[0], args[1], args[2], res, args[3]);
    } else if (strcmp(command, "search_grid_entity_1") == 0) {
        t_ivec3 pos;
        glmc_ivec3_set(pos, args[0], args[1], args[2]);
        search_grid_entity_1(engine, pos, args[3], args[4], res, args[5]);
    } else if (strcmp(command, "search_grid_entity_2") == 0) {
        t_ivec3 pos;
        glmc_ivec3_set(pos, args[0], args[1], args[2]);
        search_grid_entity_2(engine, pos, args[3], args[4], args[5], res, args[6]);
    } else if (strcmp(command, "search_first_entity_with_mask") == 0) {
        // do nothing, dummy trace
    } else if (strcmp(command, "search_first_entity_1") == 0) {
        search_first_entity_1(engine, args[0], args[1]);
        iteratorInit(engine, set_requires_component(0, args[0]), args[1], res);
    } else if (strcmp(command, "search_first_entity_2") == 0) {
        search_first_entity_2(engine, args[0], args[1], args[2]);
        component_id_filter_t component_id_filter = set_requires_component(0, args[0]);
        component_id_filter = set_requires_component(component_id_filter, args[1]);
        iteratorInit(engine, component_id_filter, args[2], res);
    } else if (strcmp(command, "search_first_entity_3") == 0) {
        search_first_entity_3(engine, args[0], args[1], args[2], args[3]);
        component_id_filter_t component_id_filter = set_requires_component(0, args[0]);
        component_id_filter = set_requires_component(component_id_filter, args[1]);
        component_id_filter = set_requires_component(component_id_filter, args[2]);
        iteratorInit(engine, component_id_filter, args[3], res);
    } else if (strcmp(command, "es_memory_manager_init") == 0) {
        es_memory_manager_init(&(engine->es_memory));
    } else {
    }
}
void iteratorInit(Engine* engine, component_id_filter_t filter, SystemId system,
                  EntityIterator* res) {
    search_entity(engine, filter, res, system);
}
