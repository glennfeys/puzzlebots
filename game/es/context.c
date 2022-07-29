#include "context.h"
#include "../../util/rgb_triple.h"

#include <stdlib.h>

void context_init(Context* c, LevelName lvl) {
    c->is_exit_game = 0;
    c->demo = 0;
    c->time = 0;
    c->lastCameraLookFrom[0] = 0;
    c->lastCameraLookFrom[1] = 0;
    c->lastCameraLookFrom[2] = 0;
    c->lastCameraLookAt[0] = 0;
    c->lastCameraLookAt[1] = 0;
    c->lastCameraLookAt[2] = 0;
    c->visibility_mode = ALL;
    c->controlledIndex = 1;
    c->currentLevel = lvl;
}

Context* context_alloc(LevelName lvl) {
    Context* res = malloc(sizeof(Context));
    context_init(res, lvl);
    return res;
}
void context_free(Context* c) {}
