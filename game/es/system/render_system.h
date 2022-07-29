#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

typedef struct RenderSystem RenderSystem;

#include "../../graphics/opengl_game_renderer.h"
#include "../engine.h"
#include "../../graphics/gl_tetrahedron.h"
#include "../../graphics/gl_hexprism.h"
#include "../../graphics/gl_floor.h"
#include "../../graphics/gl_exit.h"
#include "../../graphics/gl_wall.h"
#include "../../graphics/gl_actor.h"
#include "../../graphics/gl_button.h"
#include "../../graphics/gl_door.h"
#include "../../graphics/gl_teleport.h"
#include "../system_enums.h"

typedef struct RenderSystem {
    SystemId system_id;

    Graphics* graphics;

    GLTetrahedron tetrahedron;
    GLHexprism hexprism;
    GLFloor floor;
    GLExit exit;
    GLWall wall[4];
    GLActor bot;
    GLActor eye;
    GLButton button;
    GLTeleport teleport;
    GLDoor door[2];
} RenderSystem;

void system_render_init(RenderSystem*, Graphics*, Engine*);
RenderSystem* system_render_alloc(Graphics*, Engine*);
void system_render_update(RenderSystem*, Engine*);
void system_render_free(RenderSystem*);

#endif //RENDER_SYSTEM_H