#ifndef SYSPROG_GL_HEXPRISM_H
#define SYSPROG_GL_HEXPRISM_H

#include "gl_tex_object.h"

typedef struct GLHexprism {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
} GLHexprism;

void gl_hexprism_init(GLHexprism *, Graphics *);
void gl_hexprism_free(GLHexprism *);
void  gl_hexprism_draw(GLHexprism* obj, uint64_t time, t_vec3 gridPos);

#endif //SYSPROG_GL_HEXPRISM_H
