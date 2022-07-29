#ifndef ASSEMBLAGE_H
#define ASSEMBLAGE_H

#include "../level/levelloader.h"
#include "engine.h"
#include "game_util.h"
#include <glmc.h>

// example assemblage methods:

void createLevelEntities(Engine* engine, Level* level);

EntityId createFloorAndWall(Engine* engine, Level* level, LevelCell* c, t_ivec3 pos);
EntityId createBot(Engine* engine, t_ivec3 gridlocation, int controlled, int level_nr,
                   int bot_index);
EntityId createEye(Engine* engine, t_ivec3 pos, int controlled);
EntityId createWeightedTetrahedron(Engine* engine, t_ivec3 gridlocation);
EntityId createWeightedHexPrism(Engine* engine, t_ivec3 gridlocation);
EntityId createButton(Engine* engine, t_ivec3 gridlocation, EntityColor color, int requires_hold);
EntityId createDoor(Engine* engine, t_ivec3 tvec3, Direction direction, EntityColor color);
EntityId createExit(Engine* engine, t_ivec3 tvec3);
void createTeleports(Engine* engine, t_ivec3 gridlocation1, t_ivec3 gridlocation2,
                     EntityColor color);

#endif
