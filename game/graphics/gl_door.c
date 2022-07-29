#include <assert.h>
#include "gl_door.h"

void gl_door_init(GLDoor *obj, Graphics *graphics, GridAxis parallelGridAxis) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    
    obj->blend = 1;
    obj->no_cull = 1;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    if (parallelGridAxis == X) {
        for (int v = 0; v < 4; v++) {
            int i = ((quadIndex * 4) + v) * 3;
            
            obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0; //x
            obj->vertex_buffer_data[i + 1] = v % 2 == 0 ? 1.0 : -1.0; //y
            obj->vertex_buffer_data[i + 2] = 0.0; //z
            
            obj->normal_buffer_data[i] = 0.0; //x
            obj->normal_buffer_data[i + 1] = 0.0; //y
            obj->normal_buffer_data[i + 2] = -1.0; //z
            
            int j = ((quadIndex * 4) + v) * 2;
            obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
            obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
        }
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        
        quadIndex++;
    }
    
    if (parallelGridAxis == Y) {
        for (int v = 0; v < 4; v++) {
            int i = ((quadIndex * 4) + v) * 3;
            
            obj->vertex_buffer_data[i] = 0.0; //x
            obj->vertex_buffer_data[i + 1] = v < 2 ? -1.0 : 1.0; //y
            obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 1.0 : -1.0; //z
            
            obj->normal_buffer_data[i] = -1.0; //x
            obj->normal_buffer_data[i + 1] = 0.0; //y
            obj->normal_buffer_data[i + 2] = 0.0; //z
            
            int j = ((quadIndex * 4) + v) * 2;
            obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
            obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
        }
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        
        quadIndex++;
    }
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLDoor");
}

void gl_door_free(GLDoor *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_door_draw(GLDoor *obj, uint64_t time, t_vec3 gridPos, GameColor color) {
    t_texid texid = TEXTURE_DOOR_A;
    switch(color) {
        case GAME_COLOR_A: {
            texid = TEXTURE_DOOR_A;
            break;
        }
        case GAME_COLOR_C: {
            texid = TEXTURE_DOOR_B;
            break;
        }
        case GAME_COLOR_B: {
            texid = TEXTURE_DOOR_C;
            break;
        }
    }
    
    t_vec3 worldPos = {0};
    level_pos_to_world_pos3f(gridPos, worldPos);
    
    
    gl_tex_draw(&obj->gl_pt_object, worldPos, texid);
}
