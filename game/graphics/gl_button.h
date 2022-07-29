#ifndef SYSPROG_GL_BUTTON_H
#define SYSPROG_GL_BUTTON_H

#include "gl_tex_object.h"

typedef struct GLButton {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
    
    
} GLButton;

void gl_button_init(GLButton *, Graphics *);
void gl_button_free(GLButton *);
void  gl_button_draw(GLButton *obj, uint64_t time, t_vec3 gridPos, GameColor buttonColor, int onceButton, int active);

#endif //SYSPROG_GL_BUTTON_H
