#include <assert.h>
#include "gl_tetrahedron.h"

#define GLTETR_SIZE 0.4f

void gl_tetrahedron_init(GLTetrahedron *obj, Graphics *graphics) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    
    obj->blend = 1;
    
    int indicesIndex = 0;
    
    //we can make a tetrahedron using 4 corners of a cube
    int t = 0;
    obj->vertex_buffer_data[t] = -GLTETR_SIZE;
    obj->vertex_buffer_data[t + 1] = -GLTETR_SIZE;
    obj->vertex_buffer_data[t + 2] = -GLTETR_SIZE;
    t+=3;
    obj->vertex_buffer_data[t] = GLTETR_SIZE;
    obj->vertex_buffer_data[t + 1] = -GLTETR_SIZE;
    obj->vertex_buffer_data[t + 2] = GLTETR_SIZE;
    t+=3;
    obj->vertex_buffer_data[t] = GLTETR_SIZE;
    obj->vertex_buffer_data[t + 1] = GLTETR_SIZE;
    obj->vertex_buffer_data[t + 2] = -GLTETR_SIZE;
    t+=3;
    obj->vertex_buffer_data[t] = -GLTETR_SIZE;
    obj->vertex_buffer_data[t + 1] = GLTETR_SIZE;
    obj->vertex_buffer_data[t + 2] = GLTETR_SIZE;
    
    
    t+=3;
    assert(t == 3*4);
    
    //texCoords
    int tc = 0;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 0.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 0.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 1.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 1.0;
    tc+=2;
    
    for (int i = 0; i < 3*4; i += 3) {
        //for now give everything point up as normal
        obj->normal_buffer_data[i] = 0.0f; //x
        obj->normal_buffer_data[i + 1] = -1.0f; //y
        obj->normal_buffer_data[i + 2] = 0.0f; //z
    }
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 2;
    obj->indices[indicesIndex++] = 1;
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 3;
    obj->indices[indicesIndex++] = 2;
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 1;
    obj->indices[indicesIndex++] = 3;
    
    obj->indices[indicesIndex++] = 1;
    obj->indices[indicesIndex++] = 2;
    obj->indices[indicesIndex++] = 3;
    
    assert((indicesIndex / 3) == 4);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, 4, "GLTetrahedron");
}

void gl_tetrahedron_free(GLTetrahedron *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_tetrahedron_draw(GLTetrahedron* obj, uint64_t time, t_vec3 gridPos) {
    t_vec3 worldPos;
    level_pos_to_world_pos3f(gridPos, worldPos);
    
    gl_tex_draw(&obj->gl_pt_object, worldPos, TEXTURE_TETRAHEDRON);
}
