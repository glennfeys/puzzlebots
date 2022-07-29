#ifndef SYSPROG_GL_PLAYER_H
#define SYSPROG_GL_PLAYER_H

#include "gl_tex_object.h"

typedef struct GLActor {
#ifdef __cplusplus
    struct GLTexObject parent;
#else
    union {
        struct GLTexObject;
        GLTexObject gl_pt_object;
    };
#endif
    float scale;
} GLActor;

#define GLACTOR_BOT_SCALE 0.6f
#define GLACTOR_EYE_SCALE 0.25f

void gl_actor_init(GLActor *obj, Graphics *graphics, float scale);
void gl_actor_free(GLActor *obj);
void  gl_actor_draw(GLActor *obj, uint64_t time, t_vec3 gridPos, float rotation_angle, int bot, int controlled, int unique_id);

#endif //SYSPROG_GL_PLAYER_H
