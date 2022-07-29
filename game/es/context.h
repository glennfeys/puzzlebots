#ifndef CONTEXT_H
#define CONTEXT_H

#include "../level/levelloader.h"
#include <glmc.h>
#include <stdint.h>

typedef enum VisibilityMode { ALL, SHADE_OLD, ONLY_DIRECT } VisibilityMode;

typedef struct Context {
    uint64_t time;
    int is_exit_game;
    int demo;
    t_vec3 lastCameraLookAt;
    t_vec3 lastCameraLookFrom;
    VisibilityMode visibility_mode;
    int controlledIndex;
    LevelName currentLevel;
    // TODO: add other context fields (if any)
} Context;

void context_init(Context*, LevelName);
Context* context_alloc(LevelName);
void context_free(Context*);

#endif // CONTEXT_H
