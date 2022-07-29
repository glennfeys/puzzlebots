#ifndef SYSPROG_GL_TETRAHEDRON_H
#define SYSPROG_GL_TETRAHEDRON_H

#include "gl_tex_object.h"

typedef struct GLTetrahedron {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
} GLTetrahedron;

void gl_tetrahedron_init(GLTetrahedron *, Graphics *);
void gl_tetrahedron_free(GLTetrahedron *);
void  gl_tetrahedron_draw(GLTetrahedron* obj, uint64_t time, t_vec3 gridPos);

#endif //SYSPROG_GL_TETRAHEDRON_H
