#include "levelloader.h"
#include "../../../util/util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <time.h>

Level* level_alloc(unsigned int width, unsigned int height, unsigned int nr) {
    assert(width > 0);
    assert(height > 0);
    assert(width < LEVEL_MAX_WIDTH);
    assert(height < LEVEL_MAX_HEIGHT);
    Level* res = malloc(sizeof(Level));
    level_init(res, width, height, nr);
    return res;
}

void level_free(Level* ll) {}

void level_init(Level* level, unsigned int width, unsigned int height, unsigned int nr) {
    level->width = width;
    level->height = height;
    level->nr = nr;

    for (int x = 0; x < level->width; x++) {
        for (int y = 0; y < level->height; y++) {
            level->cells[x][y].content_mask = 0;
            level->cells[x][y].color = ECOLOR_A;
            level->cells[x][y].z = 0;
        }
    }
}

void levelloader_free_level(Level* l) {
    level_free(l);
    free(l);
}

void levelloader_free(LevelLoader* ll) { free(ll); }

int bound_rand(int first_incl, int last_incl) {
    if (first_incl == last_incl)
        return first_incl;
    assert(first_incl < last_incl);
    int r = rand(); // between 0 and RAND_MAX
    int res = (r % (last_incl - first_incl) + 1) + first_incl;
    assert(res >= first_incl);
    assert(res <= last_incl);
    return res;
}
int rand_bool() {
    int r = rand(); // between 0 and RAND_MAX
    return r & 0x10;
}

Level* generate_level_testing(unsigned int level_nr) {
    unsigned int w = 2;
    unsigned int h = 2;
    Level* level = level_alloc(w, h, level_nr);
    level->cells[0][0].z = 0;
    level->cells[0][0].content_mask = CELL_START;
    level->cells[0][1].z = 0;
    level->cells[0][1].content_mask = CELL_BUTTON_ONCE;
    level->cells[0][1].color = ECOLOR_B;

    return level;
}

Level* generate_level_testgravity(unsigned int level_nr) {
    unsigned int w = 5;
    unsigned int h = 5;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    for (int x = 0; x < w; x++) {
        level->cells[x][0].z = 1;
    }
    for (int x = 1; x < w; x++) {
        level->cells[x][2].z = -1;
    }
    for (int x = 1; x < w; x++) {
        level->cells[x][2].z = -2;
    }

    level->cells[2][0].content_mask |= CELL_START;
    level->cells[2][h - 1].content_mask |= CELL_EXIT;

    return level;
}

Level* generate_level_test(unsigned int level_nr) {
    unsigned int w = 10;
    unsigned int h = 10;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    for (int x = 0; x < 6; x++) {
        level->cells[x][7].z = 1;
    }
    for (int x = 1; x < 6; x++) {
        level->cells[x][6].z = -1;
    }

    level->cells[3][7].z = 0;

    level->cells[7][3].z = 1;
    level->cells[7][5].z = -1;

    level->cells[9][3].z = 2;
    level->cells[9][5].z = -2;

    level->cells[1][4].content_mask |= CELL_DOOR;
    level->cells[1][4].color = ECOLOR_A;
    level->cells[3][4].content_mask |= CELL_DOOR;
    level->cells[3][4].color = ECOLOR_B;
    level->cells[5][4].content_mask |= CELL_DOOR;
    level->cells[5][4].color = ECOLOR_C;

    level->cells[1][3].content_mask |= CELL_BUTTON_ONCE;
    level->cells[1][3].color = ECOLOR_A;
    level->cells[3][3].content_mask |= CELL_BUTTON_ONCE;
    level->cells[3][3].color = ECOLOR_B;
    level->cells[5][3].content_mask |= CELL_BUTTON_HOLD;
    level->cells[5][3].color = ECOLOR_C;

    level->cells[1][2].content_mask |= CELL_BUTTON_ONCE;
    level->cells[1][2].color = ECOLOR_A;
    level->cells[3][2].content_mask |= CELL_BUTTON_HOLD;
    level->cells[3][2].color = ECOLOR_B;
    level->cells[5][2].content_mask |= CELL_BUTTON_HOLD;
    level->cells[5][2].color = ECOLOR_C;

    level->cells[1][6].content_mask |= CELL_TELEPORT;
    level->cells[1][6].color = ECOLOR_A;
    level->cells[4][7].content_mask |= CELL_TELEPORT;
    level->cells[4][7].color = ECOLOR_A;

    level->cells[9][0].content_mask |= CELL_TELEPORT;
    level->cells[9][0].color = ECOLOR_C;
    level->cells[9][8].content_mask |= CELL_TELEPORT;
    level->cells[9][8].color = ECOLOR_C;

    level->cells[7][2].content_mask |= CELL_WEIGHTED_HEXPRISM;
    level->cells[9][2].content_mask |= CELL_WEIGHTED_TETRAHEDRON;

    for (int x = 0; x < w; x++) {
        level->cells[x][h - 1].content_mask |= CELL_EXIT;
    }
    level->cells[9][5].content_mask |= CELL_EXIT;

    for (int start_count = 0; start_count < 3; start_count++) {
        level->cells[start_count][0].content_mask |= CELL_START;
    }

    level->cells[0][0].content_mask |= CELL_START;
    level->cells[0][0].z = 1;
    level->cells[0][1].z = 1;
    level->cells[0][1].content_mask |= CELL_WEIGHTED_HEXPRISM;

    return level;
}

Level* generate_empty_level(unsigned int level_nr) {
    unsigned int w = 15 + level_nr;
    unsigned int h = 15 + level_nr;
    unsigned int mid_w = w / 2;
    unsigned int mid_h = h / 2;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    level->cells[w - 1][h - 1].content_mask |= CELL_EXIT;
    level->cells[mid_w][mid_h].content_mask |= CELL_START;

    return level;
}

Level* generate_level_wallgap(unsigned int level_nr) {
    unsigned int w = 7 + (bound_rand(0, 3) * 2);
    unsigned int h = 9 + (bound_rand(0, 3) * 2);
    unsigned int mid_w = bound_rand(0, 2) + (w / 2) - 1;
    unsigned int mid_h = bound_rand(0, 2) + (h / 2) - 1;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    for (int x = 0; x < w; x++) {
        level->cells[x][mid_h].z = 1;
    }

    level->cells[bound_rand(0, w - 1)][mid_h].z = 0;

    for (int x = 0; x < w; x++) {
        level->cells[x][h - 1].content_mask |= CELL_EXIT;
    }

    int bot_count = bound_rand(1, 3);
    for (int start_count = 0; start_count < bot_count; start_count++) {
        level->cells[bound_rand(0, w - 1)][0].content_mask |= CELL_START;
    }

    return level;
}

Level* generate_level_bridge(unsigned int level_nr) {
    int w = 7 + (bound_rand(0, 3) * 2);
    int h = 9 + (bound_rand(0, 3) * 2);
    int mid_w = w / 2;
    int mid_h = h / 2;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    for (int x = 0; x < w; x++) {
        level->cells[x][mid_h - 1].z = -1;
        level->cells[x][mid_h].z = -2;
        level->cells[x][mid_h + 1].z = -1;
    }

    int has_bridge = 0;
    if (rand_bool()) {
        level->cells[mid_w - 1][mid_h - 1].z = 0;
        level->cells[mid_w - 1][mid_h].z = 0;
        level->cells[mid_w - 1][mid_h + 1].z = 0;
        has_bridge = 1;
    }
    if (rand_bool()) {
        level->cells[mid_w][mid_h - 1].z = 0;
        level->cells[mid_w][mid_h].z = 0;
        level->cells[mid_w][mid_h + 1].z = 0;
        has_bridge = 1;
    }
    if (rand_bool() || !has_bridge) {
        level->cells[mid_w + 1][mid_h - 1].z = 0;
        level->cells[mid_w + 1][mid_h].z = 0;
        level->cells[mid_w + 1][mid_h + 1].z = 0;
    }

    int exit_y = bound_rand(1, mid_h - 3);
    int start_y = bound_rand(mid_h + 2, h - 1);
    for (int x = 0; x < w; x++) {
        if (x % 2 == 1) {
            level->cells[x][exit_y].content_mask |= CELL_EXIT;
        } else {
            level->cells[x][start_y].content_mask |= CELL_START;
        }
    }

    return level;
}

Level* generate_level_hexprism_bridge(unsigned int level_nr) {
    int w = 7 + (bound_rand(0, 3) * 2);
    int h = 9 + (bound_rand(0, 3) * 2);
    int mid_w = bound_rand(0, 2) + (w / 2) - 1;
    int mid_h = bound_rand(0, 2) + (h / 2) - 1;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    for (int x = 0; x < w; x++) {
        level->cells[x][mid_h].z = -1;
    }

    level->cells[3][bound_rand(3, mid_h - 2)].content_mask |= CELL_EXIT;

    for (int start_count = 0; start_count < bound_rand(1, 4); start_count++) {
        level->cells[bound_rand(0, w - 1)][h - 2].content_mask |= CELL_START;
    }

    for (int i = 2; i < w; ++i) {
        level->cells[i][mid_h + 4].z = 1;
    }
    for (int j = 1; j < 4; ++j) {
        level->cells[2][mid_h + j].z = 1;
    }

    level->cells[0][mid_h + 2].content_mask |= CELL_WEIGHTED_HEXPRISM;
    level->cells[w - 1][mid_h - 3].content_mask |= CELL_WEIGHTED_HEXPRISM;
    level->cells[3][mid_h + 2].content_mask |= CELL_WEIGHTED_HEXPRISM;

    return level;
}

#define LEVEL_3_MAZE_MIN_W 11
#define LEVEL_3_MAZE_MIN_H 11
#define LEVEL_3_MAZE_MAX_W 21
#define LEVEL_3_MAZE_MAX_H 21
#define LEVEL_3_WALL_MAX                                                                           \
    ((2 * (LEVEL_3_MAZE_MAX_W / 2) * ((LEVEL_3_MAZE_MAX_H - 2) / 2)) -                             \
     ((LEVEL_3_MAZE_MAX_H - 2) / 2 + LEVEL_3_MAZE_MAX_W / 2))

typedef struct coord {
    int x, y;
} coord;

coord check_wall(Level* level, int cell_x, int cell_y, Direction dir) {
    assert(cell_y > 0);
    assert(cell_y < level->height - 1);
    assert(cell_x >= 0);
    assert(cell_x <= level->width);
    assert(cell_x % 2 == 1);
    assert(cell_y % 2 == 0);

    coord wall_coord;
    wall_coord.x = dir == E ? cell_x + 1 : (dir == W ? cell_x - 1 : cell_x);
    wall_coord.y = dir == S ? cell_y + 1 : (dir == N ? cell_y - 1 : cell_y);
    if (wall_coord.x < 1 || wall_coord.x > level->width - 1) {
        wall_coord.x = 0;
        wall_coord.y = 0;
        return wall_coord;
    }
    if (wall_coord.x == level->width - 1 && wall_coord.x % 2 == 0) {
        wall_coord.x = 0;
        wall_coord.y = 0;
        return wall_coord;
    }
    if (wall_coord.y < 2 || wall_coord.y > level->height - 2) {
        wall_coord.x = 0;
        wall_coord.y = 0;
        return wall_coord;
    }
    if (wall_coord.y == level->height - 2 && wall_coord.y % 2 == 1) {
        wall_coord.x = 0;
        wall_coord.y = 0;
        return wall_coord;
    }
    if (level->cells[wall_coord.x][wall_coord.y].z != 0)
        return wall_coord;
    else {
        wall_coord.x = 0;
        wall_coord.y = 0;
        return wall_coord;
    }
}

int is_cell_visited(Level* level, int cell_x, int cell_y) {
    //    if (cell_y >= level->height-1)
    //        return 1;
    //    if (cell_y < 1)
    //        return 1;
    assert(cell_y > 0);
    assert(cell_y < level->height - 1);
    assert(cell_x >= 0);
    assert(cell_x <= level->width);
    assert(cell_x % 2 == 1);
    assert(cell_y % 2 == 0);
    return level->cells[cell_x][cell_y].z == 0;
}
void visit_cell(Level* level, int cell_x, int cell_y, coord* cur_wall_ids, int* cur_wall_count) {
    assert(cell_y > 0);
    assert(cell_y < level->height - 1);
    assert(cell_x >= 0);
    assert(cell_x <= level->width);
    assert(cell_x % 2 == 1);
    assert(cell_y % 2 == 0);

    level->cells[cell_x][cell_y].z = 0;
    // check the walls in all directions, if they are at valid position, and not yet removed, add
    // them to wall list
    for (Direction dir = DIRECTION_FIRST; dir <= DIRECTION_LAST; dir++) {
        coord wall_coord = check_wall(level, cell_x, cell_y, dir);
        if (wall_coord.x == 0)
            continue;
        cur_wall_ids[*cur_wall_count] = wall_coord;
        *cur_wall_count += 1;
    }
}

Level* generate_level_maze(unsigned int level_nr) {
    int w = LEVEL_3_MAZE_MIN_W + bound_rand(0, (LEVEL_3_MAZE_MAX_W - LEVEL_3_MAZE_MIN_W) / 2) * 2;
    int h = LEVEL_3_MAZE_MIN_H + bound_rand(0, (LEVEL_3_MAZE_MAX_H - LEVEL_3_MAZE_MIN_H) / 2) * 2;
    int bottom_h = 1;
    int top_h = h - 2;

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = y == h - 1 || y == 0 ? 0 : 1;
        }
    }

    level->cells[bound_rand(0, w - 1)][0].content_mask |= CELL_EXIT;
    level->cells[bound_rand(0, w - 1)][h - 1].content_mask |= CELL_START;

    // as in this maze, walls take as much space as cells, the maze has half as many rows and cols
    // as its size
    int cols = w / 2;
    int rows = (h - 2) / 2;

    // create a random entrance and exit
    int exit_a_x = 1 + bound_rand(0, (cols / 2) - 1) * 2;
    int exit_b_x = 1 + bound_rand(0, (cols / 2) - 1) * 2;
    level->cells[exit_a_x][h - 2].z = 0;
    level->cells[exit_b_x][1].z = 0;

    // initialise wall list
    coord cur_wall_ids[LEVEL_3_WALL_MAX];
    // start with the 2 walls of the cell at (1, 2) in the wall list
    int cur_wall_count = 2;
    cur_wall_ids[0].x = 2;
    cur_wall_ids[0].y = 2;
    cur_wall_ids[1].x = 1;
    cur_wall_ids[1].y = 3;

    // prim's randomized algo
    while (cur_wall_count > 0) {
        // pop a random wall from the wall list
        int random_pos = bound_rand(0, cur_wall_count - 1);
        coord chosen = cur_wall_ids[random_pos];
        cur_wall_ids[random_pos] = cur_wall_ids[cur_wall_count - 1];
        cur_wall_count--;

        // check the 2 cells connected by the wall.
        // if a cell is not yet visited, remove the wall, and visit the cell
        if (chosen.y % 2 == 0) {
            if (!is_cell_visited(level, chosen.x - 1, chosen.y)) {
                visit_cell(level, chosen.x - 1, chosen.y, cur_wall_ids, &cur_wall_count);
                level->cells[chosen.x][chosen.y].z = 0;
            }
            if (!is_cell_visited(level, chosen.x + 1, chosen.y)) {
                visit_cell(level, chosen.x + 1, chosen.y, cur_wall_ids, &cur_wall_count);
                level->cells[chosen.x][chosen.y].z = 0;
            }
        } else {
            if (!is_cell_visited(level, chosen.x, chosen.y - 1)) {
                visit_cell(level, chosen.x, chosen.y - 1, cur_wall_ids, &cur_wall_count);
                level->cells[chosen.x][chosen.y].z = 0;
            }
            if (!is_cell_visited(level, chosen.x, chosen.y + 1)) {
                visit_cell(level, chosen.x, chosen.y + 1, cur_wall_ids, &cur_wall_count);
                level->cells[chosen.x][chosen.y].z = 0;
            }
        }
    }

    return level;
}

Level* generate_level_gap_to_open_door(unsigned int level_nr) {
    unsigned int w = 7 + (bound_rand(0, 3) * 2);
    unsigned int h = 9 + (bound_rand(0, 3) * 2);
    int mid_w = bound_rand(0, 2) + (w / 2) - 1;
    int mid_h = bound_rand(0, 2) + (h / 2) - 1;
    int tq_h = bound_rand(mid_h + 2, h - 2);

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    level->cells[0][0].z = 1;

    for (int x = 0; x < mid_w; x++) {
        level->cells[x][mid_h].z = -1;
    }
    for (int y = 0; y < h; y++) {
        level->cells[mid_w][y].z = 1;
    }
    level->cells[mid_w - 1][mid_h].content_mask |= CELL_TELEPORT;
    level->cells[mid_w - 1][mid_h].color = ECOLOR_A;
    level->cells[0][0].content_mask |= CELL_TELEPORT;
    level->cells[0][0].color = ECOLOR_A;

    // fixed location for button to keep it simple
    level->cells[0][h - 1].content_mask |= CELL_BUTTON_ONCE;
    level->cells[0][h - 1].color = ECOLOR_B;
    // harder:    level->cells[mid_w/2][tq_h].content_mask |= CELL_BUTTON_ONCE;
    //           level->cells[mid_w/2][tq_h].color = ECOLOR_B;

    int door_y = bound_rand(0, mid_h - 1);
    level->cells[mid_w][door_y].content_mask |= CELL_DOOR;
    level->cells[mid_w][door_y].color = ECOLOR_B;
    level->cells[mid_w][door_y].z = 0;

    level->cells[bound_rand(mid_w + 1, w - 1)][bound_rand(mid_h + 2, h - 2)].content_mask |=
        CELL_EXIT;

    for (int start_count = 0; start_count < bound_rand(1, 5); start_count++) {
        level->cells[bound_rand(1, mid_w - 2)][bound_rand(1, mid_h - 2)].content_mask |= CELL_START;
    }

    return level;
}

Level* generate_level_4_rooms(unsigned int level_nr) {
    unsigned int w = 8 + (bound_rand(0, 3) * 2);
    unsigned int h = 9 + (bound_rand(0, 3) * 2);
    int mid_w = bound_rand(4, w - 4);
    int mid_h = bound_rand(4, h - 4);

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    int bot_count = bound_rand(4, 6);
    for (int y = 0; y < bot_count; y++) {
        level->cells[bound_rand(0, mid_w - 2)][y].content_mask |= CELL_START;
    }

    for (int x = 0; x < w; x++) {
        level->cells[x][mid_h].z = -1;
    }
    for (int y = 0; y < h; y++) {
        level->cells[mid_w][y].z = 1;
    }

    level->cells[mid_w / 2][mid_h / 2].content_mask |= CELL_BUTTON_HOLD;
    level->cells[mid_w / 2][mid_h / 2].color = ECOLOR_A;
    level->cells[mid_w / 2][3 * mid_h / 2].content_mask |= CELL_BUTTON_HOLD;
    level->cells[mid_w / 2][3 * mid_h / 2].color = ECOLOR_B;
    level->cells[3 * mid_w / 2][3 * mid_h / 2].content_mask |= CELL_BUTTON_HOLD;
    level->cells[3 * mid_w / 2][3 * mid_h / 2].color = ECOLOR_C;
    level->cells[3 * mid_w / 2][mid_h / 2].content_mask |= CELL_WEIGHTED_TETRAHEDRON;

    int doora_x = bound_rand(0, mid_w - 1);
    int doorb_y = bound_rand(mid_h + 1, h - 1);
    int doorc_y = bound_rand(0, mid_h - 1);
    level->cells[doora_x][mid_h].content_mask |= CELL_DOOR;
    level->cells[doora_x][mid_h].color = ECOLOR_A;
    level->cells[doora_x][mid_h].z = 0;
    level->cells[mid_w][doorb_y].content_mask |= CELL_DOOR;
    level->cells[mid_w][doorb_y].color = ECOLOR_B;
    level->cells[mid_w][doorb_y].z = 0;
    level->cells[mid_w][doorc_y].content_mask |= CELL_DOOR;
    level->cells[mid_w][doorc_y].color = ECOLOR_C;
    level->cells[mid_w][doorc_y].z = 0;

    level->cells[bound_rand(mid_w + 2, w - 2)][bound_rand(2, (mid_h / 2) - 1)].content_mask |=
        CELL_EXIT;

    return level;
}

Level* generate_level_tetrapuzzle(unsigned int level_nr) {
    unsigned int w = 8 + (bound_rand(0, 3) * 2);
    unsigned int h = 9 + (bound_rand(0, 3) * 2);
    int mid_w = bound_rand(4, w - 4);
    int mid_h = bound_rand(4, h - 4);

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    level->cells[bound_rand(0, mid_w - 2)][bound_rand(0, 3)].content_mask |= CELL_START;

    int bot2_x = bound_rand(mid_w + 2, w - 1);
    int bot2_y = bound_rand(0, 3);
    level->cells[bot2_x][bot2_y].content_mask |= CELL_START;

    for (int x = 0; x < w; x++) {
        level->cells[x][mid_h].z = 1;
    }

    for (int y = 0; y < mid_h; y++) {
        level->cells[mid_w][y].z = 1;
    }

    int button_location = bound_rand(0, mid_h - 1);
    level->cells[mid_w][button_location].z = 0;

    level->cells[mid_w][button_location].content_mask |= CELL_BUTTON_HOLD;
    level->cells[mid_w / 2][mid_h / 2].color = ECOLOR_A;

    int doora_x = bound_rand(0, mid_w - 1);
    level->cells[doora_x][mid_h].content_mask |= CELL_DOOR;
    level->cells[doora_x][mid_h].color = ECOLOR_A;
    level->cells[doora_x][mid_h].z = 0;

    level->cells[bot2_x][bound_rand(bot2_y + 1, mid_h - 1)].content_mask |=
        CELL_WEIGHTED_TETRAHEDRON;

    for (int x = 0; x < w; x++) {
        level->cells[x][h - 1].content_mask |= CELL_EXIT;
    }

    return level;
}

Level* generate_teleport_room(unsigned int level_nr) {
    unsigned int w = 7 + (bound_rand(0, 3) * 2);
    unsigned int h = 9 + (bound_rand(0, 3) * 2);
    int mid_w = bound_rand(0, 1) + (w / 2) - 1;
    int mid_h = bound_rand(0, 2) + (h / 2) - 1;
    int tq_h = bound_rand(mid_h + 2, h - 2);

    Level* level = level_alloc(w, h, level_nr);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            level->cells[x][y].z = 0;
        }
    }

    for (int i = 0; i < w; ++i) {
        level->cells[i][mid_h - 2].z = -1;
        level->cells[i][mid_h - 1].z = -1;
        level->cells[i][mid_h].z = -1;
        level->cells[i][mid_h + 1].z = -1;
    }

    for (int y = 0; y < h; y++) {
        level->cells[mid_w][y].z = 1;
    }

    int block_count_left = bound_rand(1, 3);
    for (int j = 0; j < block_count_left; ++j) {
        level->cells[bound_rand(mid_w / 2 - 1, mid_w / 2)][bound_rand(0, h - 1)].z = 1;
    }

    int block_count_right = bound_rand(1, 3);
    for (int j = 0; j < block_count_right; ++j) {
        level->cells[bound_rand(mid_w * 1.5 - 1, mid_w * 1.5)][bound_rand(0, h - 2)].z = 1;
    }

    level->cells[0][0].content_mask |= CELL_START;
    level->cells[w - 1][h - 1].content_mask |= CELL_EXIT;

    // Start to middle left
    level->cells[mid_w - 1][mid_h].content_mask |= CELL_TELEPORT;
    level->cells[mid_w - 1][mid_h].color = ECOLOR_A;
    level->cells[mid_w - 1][0].content_mask |= CELL_TELEPORT;
    level->cells[mid_w - 1][0].color = ECOLOR_A;
    // Middle left to right middle
    level->cells[0][mid_h + 1].content_mask |= CELL_TELEPORT;
    level->cells[0][mid_h + 1].color = ECOLOR_B;
    level->cells[w - 1][mid_h - 2].content_mask |= CELL_TELEPORT;
    level->cells[w - 1][mid_h - 2].color = ECOLOR_B;
    // Middle right to top left
    level->cells[mid_w + 1][mid_h - 1].content_mask |= CELL_TELEPORT;
    level->cells[mid_w + 1][mid_h - 1].color = ECOLOR_C;
    level->cells[0][h - 1].content_mask |= CELL_TELEPORT;
    level->cells[0][h - 1].color = ECOLOR_C;
    // Top left to bottom right
    level->cells[mid_w - 1][h - 3].content_mask |= CELL_TELEPORT;
    level->cells[mid_w - 1][h - 3].color = ECOLOR_D;
    level->cells[mid_w + 2][0].content_mask |= CELL_TELEPORT;
    level->cells[mid_w + 2][0].color = ECOLOR_D;
    // Bottom right to top right
    level->cells[w - 1][2].content_mask |= CELL_TELEPORT;
    level->cells[w - 1][2].color = ECOLOR_E;
    level->cells[mid_w + 1][h - 2].content_mask |= CELL_TELEPORT;
    level->cells[mid_w + 1][h - 2].color = ECOLOR_E;

    return level;
}

LevelLoader* levelloader_alloc() {
    LevelLoader* res = malloc(sizeof(LevelLoader));
    srand(time(NULL));
    res->level_count = 6;
    return res;
}

Level* levelloader_load_level(LevelLoader* ll, LevelName lvl) {
    switch (lvl) {
    case LVL_WALLGAP:
        return generate_level_wallgap(lvl);
    case LVL_BRIDGE:
        return generate_level_bridge(lvl);
    case LVL_MAZE:
        return generate_level_maze(lvl);
    case LVL_HEXPRISM_BRIDGE:
        return generate_level_hexprism_bridge(lvl);
    case LVL_TETRAPUZZLE:
        return generate_level_tetrapuzzle(lvl);
    case LVL_TELE_ROOM:
        return generate_teleport_room(lvl);
    case LVL_TEST_1:
        return generate_level_testing(lvl);
    case LVL_TEST_2:
        return generate_level_test(lvl);
    default: {
        fprintf(stderr, "Unknown level %d\n", lvl);
        exit(1);
    }
    }
}

LevelCell* levelcell_get_neighbor(Level* level, t_ivec2 pos, Direction orientation) {
    int x = pos[0];
    int y = pos[1];

    switch (orientation) {
    case N:
        if (y + 1 >= level->height) {
            return NULL;
        }
        return &(level->cells[x][y + 1]);
        break;
    case S:
        if (y - 1 < 0) {
            return NULL;
        }
        return &(level->cells[x][y - 1]);
        break;
    case E:
        if (x + 1 >= level->width) {
            return NULL;
        }
        return &(level->cells[x + 1][y]);
        break;
    case W:
        if (x - 1 < 0) {
            return NULL;
        }
        return &(level->cells[x - 1][y]);
        break;
    default:
        return NULL;
        break;
    }
}

int levelcell_wall_height(Level* level, t_ivec2 pos, Direction orientation) {
    LevelCell* neighbor;

    neighbor = levelcell_get_neighbor(level, pos, orientation);
    int my_z = level->cells[pos[0]][pos[1]].z;

    if (neighbor == NULL) {
        return 1;
    } else {
        if (neighbor->z > my_z) {
            return neighbor->z - my_z;
        } else {
            return 0;
        }
    }
}
