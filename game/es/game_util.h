#ifndef SYSPROG_GAME_UTIL_H
#define SYSPROG_GAME_UTIL_H

#include <stdint.h>
#include <assert.h>

#include "../../util/util.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Direction definition:
 *
 *        OpenGL           game grid       game w or h
 * N -> towards min Z -> towards max y  -> getHeight()
 * S -> towards max Z -> towards min y  -> getHeight()
 * E -> towards max X -> towards max x  -> getWidth()
 * W -> towards min X -> towards min x  -> getWidth()
 * */

//#define N 0u
//#define W 1u
//#define S 2u
//#define E 3u
//typedef uint8_t Direction;
//#define DIRECTION_COUNT 4


typedef enum Direction { N, W, S, E, DIRECTION_COUNT } Direction;

#define DIRECTION_FIRST N
#define DIRECTION_LAST E

static_assert(N == 0, "North must be 0");
static_assert(W == 1, "West must be 1");
static_assert(S == 2, "South must be 2");
static_assert(E == 3, "East must be 3");
static_assert(DIRECTION_COUNT == 4, "DIRECTION_COUNT must be 4");
static_assert(DIRECTION_FIRST == 0, "DIRECTION_FIRST must be 0");
static_assert(DIRECTION_LAST == DIRECTION_COUNT-1, "DIRECTION_LAST must be last");

#define OPPOSITE_DIRECTION_2D(DIR) ((((DIR) % DIRECTION_COUNT) + 2) % DIRECTION_COUNT)
#define LEFT_DIRECTION_2D(DIR) ((((DIR) % DIRECTION_COUNT) + 1) % DIRECTION_COUNT)
#define RIGHT_DIRECTION_2D(DIR) ((((DIR) % DIRECTION_COUNT) + (DIRECTION_COUNT-1)) % DIRECTION_COUNT)

#define DIRECTION_TO_NORTH_FLOAT_ANGLE_RADIANS_2D(DIR) (((DIR) % DIRECTION_COUNT)*M_PI_2)

static_assert(OPPOSITE_DIRECTION_2D(N) == S, "OPPOSITE_DIRECTION_2D(N)");
static_assert(OPPOSITE_DIRECTION_2D(W) == E, "OPPOSITE_DIRECTION_2D(W)");
static_assert(OPPOSITE_DIRECTION_2D(S) == N, "OPPOSITE_DIRECTION_2D(S)");
static_assert(OPPOSITE_DIRECTION_2D(E) == W, "OPPOSITE_DIRECTION_2D(E)");

static_assert(LEFT_DIRECTION_2D(N) == W, "LEFT_DIRECTION_2D(N)");
static_assert(LEFT_DIRECTION_2D(W) == S, "LEFT_DIRECTION_2D(W)");
static_assert(LEFT_DIRECTION_2D(S) == E, "LEFT_DIRECTION_2D(S)");
static_assert(LEFT_DIRECTION_2D(E) == N, "LEFT_DIRECTION_2D(E)");

static_assert(RIGHT_DIRECTION_2D(N) == E, "RIGHT_DIRECTION_2D(N)");
static_assert(RIGHT_DIRECTION_2D(E) == S, "RIGHT_DIRECTION_2D(E)");
static_assert(RIGHT_DIRECTION_2D(S) == W, "RIGHT_DIRECTION_2D(S)");
static_assert(RIGHT_DIRECTION_2D(W) == N, "RIGHT_DIRECTION_2D(W)");

#ifdef __cplusplus
}
#endif

#endif //SYSPROG_GAME_UTIL_H
