#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include "util/sleep.h"
#include "game/graphics/opengl_game_renderer.h"
#include "game/level/levelloader.h"
#include "game/es/game.h"
#include "util/min_unit.h"

#include <SDL.h>
#undef main //Don't let SDL overwrite main definition
#include <SDL_timer.h>

static char * test_memory_manager_create_component() {
    //We create just the memory manager, no other part of Engine, Game or Graphics
    Engine engine;
    es_memory_manager_init(&engine.es_memory);
    
    //Test
    EntityId entity = get_new_entity_id(&engine);
    mu_assert(!has_component(&engine, entity, COMP_ITEM));
    create_component(&engine, entity, COMP_ITEM);
    mu_assert(has_component(&engine, entity, COMP_ITEM));
    
    //Free resources used for testing
    es_memory_manager_free(&engine.es_memory);
    return 0;
}

static char * all_tests() {
    //Basic memory manager test
    mu_run_test(test_memory_manager_create_component);
    //TODO: add more memory_manager tests

    return 0;
}


int main(int argc, char **argv) {
    printf("Running tests:\n\n");
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
        printf("\nAT LEAST ONE TEST FAILED\n");
    }
    else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", mu_tests_run);

#ifdef _WIN32
    system("pause");
#endif

    return result != 0;
}
