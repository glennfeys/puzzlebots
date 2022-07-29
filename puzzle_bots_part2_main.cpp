#include <iostream>
extern "C" {
#include "game/es/game.h"
#include "game/graphics/opengl_game_renderer.h"
#include "game/level/levelloader.h"
}
#undef main

int main(int argc, char **argv) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    struct LevelLoader* level_loader = levelloader_alloc();
    Graphics* graphics = graphics_alloc(0, 0);

    Game* game = game_alloc(graphics, LVL_START);

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

        if (diffTicks > 1000) {
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
