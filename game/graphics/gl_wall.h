#ifndef SYSPROG_GL_WALL_H
#define SYSPROG_GL_WALL_H

#include "gl_tex_object.h"

typedef struct GLWall {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
} GLWall;

void gl_wall_init(GLWall *, Graphics *, Direction direction);
void gl_wall_free(GLWall *);
void  gl_wall_draw_color(GLWall* obj, t_vec3 gridPos, int shaded);

#endif //SYSPROG_GL_WALL_H
