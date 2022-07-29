#include <assert.h>
#include "gl_floor.h"

void gl_floor_init(GLFloor *obj, Graphics *graphics) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    
    obj->defaultProceduralTextureParameterA = 0.1f;
    obj->defaultProceduralTextureID = 1.0f;
    obj->activatedProceduralTextureID = 0.0f;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;
        
        obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0; //x
        obj->vertex_buffer_data[i + 1] = -1.0; //y
        obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 1.0 : -1.0; //z
        
        obj->normal_buffer_data[i] = 0.0; //x
        obj->normal_buffer_data[i + 1] = -1.0; //y
        obj->normal_buffer_data[i + 2] = 0.0; //z
        
        int j = ((quadIndex * 4) + v) * 2;
        obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //s
        obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //t
    }
    
    obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
    
    obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    
    quadIndex++;
    
    //normally, this is used for just 1 floor
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLFloor");
}

void gl_floor_free(GLFloor *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_floor_draw_color(GLFloor *obj, t_vec3 gridPos, int highFloor, CellDetail cellDetail, float animationPosition,
                         int shaded) {
    t_texid texid = shaded ? TEXTURE_SHADE_FLOOR : TEXTURE_FLOOR;
    switch (cellDetail) {
        case ACTIVE: {
            //todo
            texid = shaded ? TEXTURE_SHADE_FLOOR : TEXTURE_FLOOR_ACTIVE;
            break;
        }
        case INACTIVE: {
            //todo
            break;
        }
        case EXIT_TOUCH :{
            if (animationPosition > 0.0f) {
                //todo
            } else {
                //todo
            }
            break;
        }
        case PLAYER_TOUCH: {
            //todo
            break;
        }
        case NONE: {
            break;
        }
    }

    t_vec3 worldPos = {0};
    level_pos_to_world_pos3f(gridPos, worldPos);

    if (highFloor) {
        worldPos[1] += 2.0f;
    }

    gl_tex_draw(&obj->gl_pt_object, worldPos, texid);
}
