#include <assert.h>
#include "gl_button.h"

#define GLBUTTON_SIZE 0.4

void gl_button_init(GLButton *obj, Graphics *graphics) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;
        
        obj->vertex_buffer_data[i] = v < 2 ? -GLBUTTON_SIZE : GLBUTTON_SIZE;
        obj->vertex_buffer_data[i + 1] = -0.85f; //y
        obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? GLBUTTON_SIZE : -GLBUTTON_SIZE;
        
        obj->normal_buffer_data[i] = 0.0f; //x
        obj->normal_buffer_data[i + 1] = -1.0f; //y
        obj->normal_buffer_data[i + 2] = 0.0f; //z
        
        int j = ((quadIndex * 4) + v) * 2;
        obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
        obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
    }
    
    obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
    
    obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    
    quadIndex++;
    
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLButton");
}

void gl_button_free(GLButton *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_button_draw(GLButton *obj, uint64_t time, t_vec3 gridPos, GameColor buttonColor, int onceButton, int active) {
    t_texid texid = TEXTURE_BUTTON_ONCE_A;
    switch(buttonColor) {
        case GAME_COLOR_A: {
            texid = onceButton ?
                    (active ? TEXTURE_BUTTON_ACTIVE_A : TEXTURE_BUTTON_ONCE_A) :
                    (active ? TEXTURE_BUTTON_ACTIVE_A : TEXTURE_BUTTON_HOLD_A);
            break;
        }
        case GAME_COLOR_C: {
            texid = onceButton ?
                    (active ? TEXTURE_BUTTON_ACTIVE_B : TEXTURE_BUTTON_ONCE_B) :
                    (active ? TEXTURE_BUTTON_ACTIVE_B : TEXTURE_BUTTON_HOLD_B);
            break;
        }
        case GAME_COLOR_B: {
            texid = onceButton ?
                    (active ? TEXTURE_BUTTON_ACTIVE_C : TEXTURE_BUTTON_ONCE_C) :
                    (active ? TEXTURE_BUTTON_ACTIVE_C : TEXTURE_BUTTON_HOLD_C);
            break;
        }
        default:
            break;
    }
    
    t_vec3 worldPos = {0};
    level_pos_to_world_pos3f(gridPos, worldPos);
    
    
    gl_tex_draw(&obj->gl_pt_object, worldPos, texid);
}
