#ifndef OPENGL_GAME_RENDERER_H
#define OPENGL_GAME_RENDERER_H

// Include GLEW
#include <GL/glew.h>

#include <glmc.h>
#include <SDL_video.h>

#include <SDL_image.h>

#include "../es/game_util.h"

typedef enum GameColor { GAME_COLOR_A, GAME_COLOR_B, GAME_COLOR_C } GameColor;
typedef enum GridAxis { X, Y } GridAxis;
typedef enum CellDetail { NONE, PLAYER_TOUCH, EXIT_TOUCH, ACTIVE, INACTIVE } CellDetail;

#define UNIFORM_XMACROS \
X(MVP) \
X(V) \
X(M) \
X(LightPosition_worldspace) \
X(PerDrawTransformation_worldspace) \
X(TimeTicks) \
X(TextureSampler)

#define ATTRIBUTE_XMACROS \
X(VertexBasePosition_modelspace) \
X(VertexNormal_modelspace) \
X(VertexTexCoord)

#define MAX_PROGRAM_COUNT 1

#define PROGRAM_GAME 0

typedef int t_texid;

#define TEXTURE_EXIT 0
#define TEXTURE_WALL 1
#define TEXTURE_FLOOR 2
#define TEXTURE_FLOOR_ACTIVE 3
#define TEXTURE_DOOR_A 4
#define TEXTURE_DOOR_B 5
#define TEXTURE_DOOR_C 6
#define TEXTURE_BUTTON_ONCE_A   7
#define TEXTURE_BUTTON_ONCE_B   8
#define TEXTURE_BUTTON_ONCE_C   9
#define TEXTURE_BUTTON_HOLD_A   10
#define TEXTURE_BUTTON_HOLD_B   11
#define TEXTURE_BUTTON_HOLD_C   12
#define TEXTURE_BUTTON_ACTIVE_A 13
#define TEXTURE_BUTTON_ACTIVE_B 14
#define TEXTURE_BUTTON_ACTIVE_C 15
#define TEXTURE_EYE 16
#define TEXTURE_EYE_CONTROLLED 17
#define TEXTURE_BOT 18
#define TEXTURE_BOT_CONTROLLED 19
#define TEXTURE_SHADE_WALL 20
#define TEXTURE_SHADE_FLOOR 21
#define TEXTURE_TELEPORT_A 22
#define TEXTURE_TELEPORT_B 23
#define TEXTURE_TELEPORT_C 24
#define TEXTURE_HEXPRISM 25
#define TEXTURE_TETRAHEDRON 26
#define TEXTURE_TELEPORT_D 27
#define TEXTURE_TELEPORT_E 28

#define TEXTURECOUNT 29

typedef struct Graphics {
    SDL_Surface* vidSurface;
    
    uint32_t width;
    uint32_t height;

    uint32_t screenMidX;
    uint32_t screenMidY;
    
    GLuint program_id[MAX_PROGRAM_COUNT];
    
    t_vec3 background_color;
    t_vec3 cameraLookFrom;
    t_vec3 cameraLookAt;
    t_vec3 cameraUp;
    
#define X(ATTRIB) GLuint uniform_##ATTRIB[MAX_PROGRAM_COUNT];
    UNIFORM_XMACROS
#undef X

#define X(ATTRIB) GLuint attribute_##ATTRIB[MAX_PROGRAM_COUNT];
    ATTRIBUTE_XMACROS
#undef X


    SDL_Surface* texture_surfaces[TEXTURECOUNT];
    GLuint texture_ids[TEXTURECOUNT];
} Graphics;

Graphics* graphics_alloc(int width, int height);
void graphics_init(Graphics*, int width, int height);
void graphics_clear(Graphics*);
void graphics_free(Graphics*);

void graphics_begin_draw(Graphics*);
void graphics_begin_draw_withlight(Graphics*, float lightPower, t_vec3 lightPos);
void graphics_end_draw(Graphics*);

int handle_gl_error(const char *location);
int handle_gl_error2(char *locationA, char *locationB);

int get_program_index(Graphics*, GLuint program_id);
GLuint get_program_id(Graphics*, int program_index);

#define HAS_ATTRIBUTE(GRAPHICS, PROGRAM_INDEX, ATRIBUTE_NAME) ((GRAPHICS)->attribute_##ATRIBUTE_NAME[PROGRAM_INDEX] != -1)
#define HAS_UNIFORM(GRAPHICS, PROGRAM_INDEX, UNIFORM_NAME) ((GRAPHICS)->uniform_##UNIFORM_NAME[PROGRAM_INDEX] != -1)

#define GET_ATTRIBUTE_ID(GRAPHICS, PROGRAM_INDEX, ATRIBUTE_NAME) ((GRAPHICS)->attribute_##ATRIBUTE_NAME[PROGRAM_INDEX])
#define GET_UNIFORM_ID(GRAPHICS, PROGRAM_INDEX, UNIFORM_NAME) ((GRAPHICS)->uniform_##UNIFORM_NAME[PROGRAM_INDEX])

void level_pos_to_world_pos3i(t_ivec3 levelPos, t_vec3 dest);
void level_pos_to_world_pos3f(t_vec3 levelPos, t_vec3 dest);

int is_sensible_world_coordinate(t_vec3 worldPos);
int is_sensible_color(t_vec4 color);

typedef GLfloat gl_color[4];
void gl_copy_color_to_from(gl_color dest, gl_color source);
void gl_set_color(gl_color dest, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

#endif //OPENGL_GAME_RENDERER_H
