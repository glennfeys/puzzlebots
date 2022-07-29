#include "game.h"
#include "assemblage.h"

#include <stdlib.h>

void game_init(Game* game, Graphics* graphics, LevelName lvl) {
    game->graphics = graphics;
    engine_init(&game->engine, graphics, lvl);
}

Game* game_alloc(Graphics* graphics, LevelName lvl) {
    Game* res = malloc(sizeof(Game));
    game_init(res, graphics, lvl);
    return res;
}

void game_free(Game* game) {
    engine_free(&game->engine);
    // don't free graphics
}

void game_load_level(Game* g, Level* l) {
    Engine* e = &(g->engine);
    createLevelEntities(e, l);
    // TODO: use assembly to create all entities needed for the level
}
