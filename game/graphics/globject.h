#ifndef GLOBJECT_H
#define GLOBJECT_H

// Include GLEW
#include <GL/glew.h>

#include "opengl_game_renderer.h"

//#define GLOBJECT_MAX_VERTICES (4*6)                         //enough for a cube with each side seperate vertices
#define GLOBJECT_MAX_VERTICES 28                            //enough for a hexprism
#define GLOBJECT_MAX_TRIANGLES (4*6*2)                      //enough for a hexprism and cube
#define GLOBJECT_MAX_INDICES (GLOBJECT_MAX_TRIANGLES * 3)   //3 points per triangle...

typedef struct GLObject {
    int program_index; //index in graphics->program_id  (not program ID in OpenGL itself, that is graphics->program_id[program_index])
    
    GLuint elementbuffer;
    int indicesCount;
    unsigned short int indices[GLOBJECT_MAX_INDICES];
} GLObject;

void globject_init(GLObject*, Graphics*, GLuint program_id);
void globject_free(GLObject *);

void globject_init_indices(GLObject*, Graphics*, int indicesCount);

#endif //GLOBJECT_H
