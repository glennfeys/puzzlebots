#ifndef SYSPROG_GL_EXIT_H
#define SYSPROG_GL_EXIT_H

#include "gl_tex_object.h"
#include "../es/game_util.h"

typedef struct GLExit {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
} GLExit;

void gl_exit_init(GLExit *, Graphics *);
void gl_exit_free(GLExit *);
void  gl_exit_draw(GLExit *obj, uint64_t time, t_vec3 gridPos);

#endif //SYSPROG_GL_EXIT_H
