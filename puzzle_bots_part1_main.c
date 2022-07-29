#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include "util/sleep.h"
#ifdef BENCHMARK
#include "util/benchmarking_utils.h"
#endif
#include "game/es/game.h"
#include "game/graphics/opengl_game_renderer.h"
#include "game/level/levelloader.h"

#include <SDL.h>
#include <SDL_timer.h>
#include <assert.h>
#undef main

void create_demo_entities(Engine* engine) {
    // As a demo, this creates a few example entities. This should be completely replaced by the
    // level_loader and assemblage in the real game
    EntityId player_entity_id = get_new_entity_id(engine);

    GridLocationComponent* gridloc = create_component(engine, player_entity_id, COMP_GRIDLOCATION);
    glmc_ivec3_set(gridloc->pos, 3, 4, 0);

    ArtComponent* player_art = create_component(engine, player_entity_id, COMP_ART);
    player_art->type = ART_PLAYER;
    DirectionComponent* player_dir = create_component(engine, player_entity_id, COMP_DIRECTION);
    player_dir->dir = E;

    CameraLookAtComponent* cameralookat =
        create_component(engine, player_entity_id, COMP_CAMERA_LOOK_AT);
    glmc_vec3_set(cameralookat->pos, 2.0f, 2.0f, 0.0f);

    CameraLookFromComponent* cameralookfrom =
        create_component(engine, player_entity_id, COMP_CAMERA_LOOK_FROM);
    cameralookfrom->distance = 10.0f;
    cameralookfrom->XYdegrees = 0.0f;
    cameralookfrom->Zdegrees = 25.0f;
    glmc_vec3_set(cameralookfrom->pos, 4.0f, -4.0f,
                  4.0f); // this normally gets overridden by camera system

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            EntityId wall_ent_id = get_new_entity_id(engine);
            ArtComponent* wall_art = create_component(engine, wall_ent_id, COMP_ART);
            wall_art->type = ART_WALL;
            GridLocationComponent* gridloc =
                create_component(engine, wall_ent_id, COMP_GRIDLOCATION);
            glmc_ivec3_set(gridloc->pos, x, y, 0);
            WallArtComponent* wallinfo = create_component(engine, wall_ent_id, COMP_WALLART);
            wallinfo->has_floor = 1;
            wallinfo->wall_height[N] = 4;
            wallinfo->wall_height[E] = x == 4;
            wallinfo->wall_height[S] = y == 0;
            wallinfo->wall_height[W] = x == 0;

            if (x == 2 && y >= 2 && y <= 3) {
                glmc_ivec3_set(gridloc->pos, x, y, -1);
                wallinfo->wall_height[N] = y == 3;
                wallinfo->wall_height[E] = 1;
                wallinfo->wall_height[S] = y == 2;
                wallinfo->wall_height[W] = 1;
            }
        }
    }

    EntityId exit_entity_id = get_new_entity_id(engine);
    GridLocationComponent* gridloc_exit =
        create_component(engine, exit_entity_id, COMP_GRIDLOCATION);
    glmc_ivec3_set(gridloc->pos, 1, 1, 0);
    ArtComponent* art_exit = create_component(engine, exit_entity_id, COMP_ART);
    art_exit->type = ART_END;
}

int main(int argc, char** argv) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

#ifdef BENCHMARK
    printf("BENCHMARKING\n");
    generate_file_name();
#endif

    struct LevelLoader* level_loader = levelloader_alloc();
    // inits the graphics system
    Graphics* graphics = graphics_alloc(0, 0);

    // initialise context, engine and assemblage, and add systems
    Game* game = game_alloc(graphics, LVL_START);

    // TODO: don't use this
    // create_demo_entities(&game->engine);

    // TODO: use the 2 lines below instead of using create_demo_entities
    Level* level = levelloader_load_level(level_loader, LVL_START);
    game_load_level(game, level);

    Uint32 startTicks = SDL_GetTicks();
    Uint32 lastPrintTicks = startTicks;
    long updateCount = 0;

    while (!game->engine.context.is_exit_game) {
        if (level->nr != game->engine.context.currentLevel) {
            LevelName nextLvl = game->engine.context.currentLevel;
            levelloader_free_level(level);
            game_free(game);
            game_init(game, graphics, nextLvl);
            level = levelloader_load_level(level_loader, nextLvl);
            game_load_level(game, level);
        }
        Uint32 curTicks = SDL_GetTicks();
        Uint32 diffTicks = curTicks - lastPrintTicks;

        engine_update(&game->engine);
        updateCount++;

        // print update count each second
        if (diffTicks > 1000) {
            // std::cout << diffTicks << "ms passed for " << updateCount << " updates" << std::endl;
            float ticksPerUpdate = (float)diffTicks / (float)updateCount;
            float fps = 1.0f / ticksPerUpdate * 1000.0f;
            printf("Ticks per update: %f  updates per sec: %f\n", ticksPerUpdate, fps);

            lastPrintTicks = curTicks;
            updateCount = 0;
        }
    }

    levelloader_free_level(level);
    levelloader_free(level_loader);

    int exit_game = game->engine.context.is_exit_game;

    game_free(game);
    free(game);

    graphics_clear(graphics);
    graphics_free(graphics);
    free(graphics);

    return 0;
}
