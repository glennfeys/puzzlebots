#include <assert.h>
#include "gl_teleport.h"

#define GLTELEPORT_SIZE 0.7f

void gl_teleport_init(GLTeleport *obj, Graphics *graphics) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    
    int indicesIndex = 0;
    
    for (int v = 0; v < 4; v++) {
        int i = v * 3;
        
        obj->vertex_buffer_data[i] = v == 0 ? -GLTELEPORT_SIZE : (v == 2 ? GLTELEPORT_SIZE : 0.0f);
        obj->vertex_buffer_data[i + 1] = -0.85f; //y
        obj->vertex_buffer_data[i + 2] = v == 1 ? GLTELEPORT_SIZE : (v == 3 ? -GLTELEPORT_SIZE : 0.0f);
        
        obj->normal_buffer_data[i] = 0.0f; //x
        obj->normal_buffer_data[i + 1] = -1.0f; //y
        obj->normal_buffer_data[i + 2] = 0.0f; //z
        
        int j = v * 2;
        obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
        obj->texCoord_buffer_data[j+1] = v ==0 || v == 3 ? 0.0 : 1.0; //w
    }
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 1;
    obj->indices[indicesIndex++] = 2;
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 2;
    obj->indices[indicesIndex++] = 3;
    
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, 4, "GLTeleport");
}

void gl_teleport_free(GLTeleport *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_teleport_draw(GLTeleport *obj, uint64_t time, t_vec3 gridPos, int teleport_id,
                      int active) {
    t_texid texid = TEXTURE_TELEPORT_A;
    switch (teleport_id) {
        case 0: {
            texid = TEXTURE_TELEPORT_A;
            break;
        }
        case 1: {
            texid = TEXTURE_TELEPORT_B;
            break;
        }
        case 2: {
            texid = TEXTURE_TELEPORT_C;
            break;
        }
        case 3: {
            texid = TEXTURE_TELEPORT_D;
            break;
        }
        case 4: {
            texid = TEXTURE_TELEPORT_E;
            break;
        }
        default: fprintf(stderr, "Bad teleport ID used: %d\n", teleport_id);
    }
    
    t_vec3 worldPos;
    level_pos_to_world_pos3f(gridPos, worldPos);
    
    
    gl_tex_draw(&obj->gl_pt_object, worldPos, texid);
}
