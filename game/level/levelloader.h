#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../es/game_util.h"
#include <glmc.h>

#define LEVEL_MAX_WIDTH 22
#define LEVEL_MAX_HEIGHT 22

// used by LevelCell content_mask
#define CELL_START 0x1u
#define CELL_EXIT 0x2u
#define CELL_BUTTON_ONCE 0x4u
#define CELL_BUTTON_HOLD 0x8u
#define CELL_DOOR 0x10u
#define CELL_TELEPORT 0x20u
#define CELL_WEIGHTED_TETRAHEDRON 0x40u
#define CELL_WEIGHTED_HEXPRISM 0x80u

typedef enum EntityColor {
    ECOLOR_A,
    ECOLOR_B,
    ECOLOR_C,
    ECOLOR_D,
    ECOLOR_E,
    ECOLOR_MAX = ECOLOR_E
} EntityColor;

typedef enum LevelName {
    LVL_WALLGAP,
    LVL_BRIDGE,
    LVL_MAZE,
    LVL_HEXPRISM_BRIDGE,
    LVL_TETRAPUZZLE,
    LVL_TELE_ROOM,
    LVL_TEST_1,
    LVL_TEST_2,
    LVL_LAST = LVL_TEST_2,
    LVL_START = LVL_WALLGAP
} LevelName;

// WEIGHTED_TETRAHEDRON can be picked up  (and doesn't block bots)
// WEIGHTED_HEXPRISM can only be pushed (and blocks bots)

typedef struct LevelCell {
    uint8_t content_mask;
    EntityColor color; // used by: door, button and teleport
    int z;
} LevelCell;

typedef struct Level {
    unsigned int width;
    unsigned int height;

    unsigned int nr;

    LevelCell cells[LEVEL_MAX_WIDTH][LEVEL_MAX_HEIGHT];
} Level;

typedef struct LevelLoader {
    unsigned int level_count;
} LevelLoader;

LevelLoader* levelloader_alloc();
void levelloader_free(LevelLoader*);

Level* levelloader_load_level(LevelLoader*, LevelName lvl);
void levelloader_free_level(Level*);

Level* level_alloc(unsigned int width, unsigned int height, unsigned int nr);
void level_init(Level*, unsigned int width, unsigned int height, unsigned int nr);
void level_free(Level*);

LevelCell* levelcell_get_neighbor(Level*, t_ivec2 pos, Direction dir);

int levelcell_wall_height(Level*, t_ivec2 pos, Direction orientation);

#endif // LEVELLOADER_H
