#ifndef GAME_H
#define GAME_H

#include "../graphics/opengl_game_renderer.h"
#include "../level/levelloader.h"
#include "engine.h"

typedef struct Game {
    Graphics* graphics;
    Engine engine;
} Game;

void game_init(Game*, Graphics*, LevelName);
Game* game_alloc(Graphics*, LevelName);
void game_free(Game*);

void game_load_level(Game*, Level*);

#endif // GAME_H
