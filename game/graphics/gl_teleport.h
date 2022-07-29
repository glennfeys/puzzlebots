#ifndef SYSPROG_GL_TELEPORT_H
#define SYSPROG_GL_TELEPORT_H

#include "gl_tex_object.h"
#include "../es/game_util.h"

typedef struct GLTeleport {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
} GLTeleport;

void gl_teleport_init(GLTeleport *, Graphics *);
void gl_teleport_free(GLTeleport *);
void  gl_teleport_draw(GLTeleport *obj, uint64_t time, t_vec3 gridPos, int teleport_id, int active);

#endif //SYSPROG_GL_TELEPORT_H
