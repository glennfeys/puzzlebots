#ifndef SYSPROG_GL_DOOR_H
#define SYSPROG_GL_DOOR_H

#include "gl_tex_object.h"

typedef struct GLDoor {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
    
    
} GLDoor;

void gl_door_init(GLDoor *, Graphics *, GridAxis parallelGridAxis);
void gl_door_free(GLDoor *);
void  gl_door_draw(GLDoor *obj, uint64_t time, t_vec3 gridPos, GameColor color);

#endif //SYSPROG_GL_DOOR_H
