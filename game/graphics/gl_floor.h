#ifndef SYSPROG_GL_FLOOR_H
#define SYSPROG_GL_FLOOR_H

#include "gl_tex_object.h"

typedef struct GLFloor {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
    
    GLfloat defaultProceduralTextureParameterA;
    GLfloat defaultProceduralTextureID;
    GLfloat activatedProceduralTextureID;
} GLFloor;

void gl_floor_init(GLFloor *, Graphics *);
void gl_floor_free(GLFloor *);
void  gl_floor_draw_color(GLFloor *obj, t_vec3 gridPos, int highFloor, CellDetail cellDetail, float animationPosition,
                          int shaded);

#endif //SYSPROG_GL_FLOOR_H
