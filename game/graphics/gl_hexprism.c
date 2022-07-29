#include <assert.h>
#include "gl_hexprism.h"

#define GLHEXP_H 0.8f
#define GLHEXP_W 0.8f

#define GLHEX_H 1.0f
#define GLHEX_W 0.866025404f

void gl_hexprism_init(GLHexprism *obj, Graphics *graphics) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    
    obj->blend = 1;
    
    int indicesIndex = 0;
    
    //bottom 7 points
    int t = 0;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = 0.0;
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = -GLHEX_H*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = -GLHEX_H/2*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = GLHEX_H/2*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = GLHEX_H*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = -GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = GLHEX_H/2*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = -GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = -GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = -GLHEX_H/2*GLHEXP_W;
    
    //top 7
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = 0.0;
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = -GLHEX_H*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = -GLHEX_H/2*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = GLHEX_H/2*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = GLHEX_H*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = -GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = GLHEX_H/2*GLHEXP_W;
    t+=3;
    obj->vertex_buffer_data[t] = -GLHEX_W*GLHEXP_W;
    obj->vertex_buffer_data[t + 1] = GLHEXP_H;
    obj->vertex_buffer_data[t + 2] = -GLHEX_H/2*GLHEXP_W;
    
    t+=3;
    assert(t == 3*14);
    
    //dummy for texCoords
    int tc = 0;
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 0.5;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 0.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 0.25;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 0.75;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 1.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 0.75;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 0.25;
    tc+=2;
    
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 0.5;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 0.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 0.25;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 0.75;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 1.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 0.75;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 0.25;
    tc+=2;
    
    for (int i = 0; i < 3*12; i += 3) {
        //for now give everything point up as normal
        obj->normal_buffer_data[i] = 0.0f; //x
        obj->normal_buffer_data[i + 1] = -1.0f; //y
        obj->normal_buffer_data[i + 2] = 0.0f; //z
    }
    
    //top and bottom triangles
    for (unsigned short int top_offset = 0; top_offset <= 7; top_offset += 7) {
        for (unsigned short int hex_index = 0; hex_index < 6; hex_index++) {
            unsigned short int center = 0;
            unsigned short int sec = 2 + hex_index;
            unsigned short int trd = 1 + hex_index;
            if (sec > 6) sec -= 6;
            if (trd > 6) trd -= 6;
            if (top_offset == 0) {
                unsigned short int tmp = sec;
                sec = trd;
                trd = tmp;
            }
            
            obj->indices[indicesIndex++] = center + top_offset;
            obj->indices[indicesIndex++] = sec + top_offset;
            obj->indices[indicesIndex++] = trd + top_offset;
        }
    }
    
    //6 sides with 2 triangles each
    for (short unsigned int side_index = 1; side_index < 7; side_index++) {
        short unsigned int start_bottom = side_index;
        short unsigned int start_top = side_index + (short unsigned int) 7;
        short unsigned int end_bottom = start_bottom + (short unsigned int) 1;
        short unsigned int end_top = start_top + (short unsigned int) 1;
        if (side_index == 6) {
            end_bottom -= 6;
            end_top -= 6;
        }
        
        obj->indices[indicesIndex++] = start_bottom;
        obj->indices[indicesIndex++] = start_top;
        obj->indices[indicesIndex++] = end_top;

        obj->indices[indicesIndex++] = start_bottom;
        obj->indices[indicesIndex++] = end_top;
        obj->indices[indicesIndex++] = end_bottom;
    }
    
    assert((indicesIndex / 3) == 6+12+6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, 14*2, "GLHexprism");
}

void gl_hexprism_free(GLHexprism *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_hexprism_draw(GLHexprism* obj, uint64_t time, t_vec3 gridPos) {
    t_vec3 worldPos;
    level_pos_to_world_pos3f(gridPos, worldPos);
    
    gl_tex_draw(&obj->gl_pt_object, worldPos, TEXTURE_HEXPRISM);
}
