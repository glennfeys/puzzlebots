#ifndef GLPTEXOBJECT_H
#define GLPTEXOBJECT_H

#include "globject.h"

#define GLLEVELPART_MAX_QUADS 6
//#define GLLEVELPART_MAX_VERTICES (GLLEVELPART_MAX_QUADS*4)
#define GLLEVELPART_MAX_VERTICES 28  //enough for hexprism



typedef struct GLTexObject {
#ifdef __cplusplus
    struct GLObject gl_object;
#else
    union {
        struct GLObject;
        GLObject gl_object;
    };
#endif
    
    Graphics* graphics;
    
    GLuint program_id;  //program ID in OpenGL   (not index in graphics->program_id, note that: program_id = graphics->program_id[program_index])
    
    int blend;
    int no_cull;
    
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint texcoordbuffer;
    
    int vertexCount;
    GLfloat vertex_buffer_data[GLLEVELPART_MAX_VERTICES*3];
    GLfloat normal_buffer_data[GLLEVELPART_MAX_VERTICES*3];
    GLfloat texCoord_buffer_data[GLLEVELPART_MAX_VERTICES*2];
} GLTexObject;

void gl_tex_object_init(GLTexObject *, Graphics *, int program_index);
void gl_tex_object_free(GLTexObject *);

void gl_tex_init_vertexbuffers(GLTexObject *, int vertexCount, char *objName);

void gl_tex_draw(GLTexObject *, t_vec3 translation, t_texid texid);
void gl_tex_draw_rotated(GLTexObject *, t_vec3 translation, float rotation_angle, t_texid texid);
void gl_tex_draw_call(GLTexObject *);


#endif //GLPTEXOBJECT_H
