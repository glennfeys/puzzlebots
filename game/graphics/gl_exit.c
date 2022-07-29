#include <assert.h>
#include "gl_exit.h"

#define GLEXIT_SIZE 0.4

void gl_exit_init(GLExit *obj, Graphics *graphics) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);

    int indicesIndex = 0;
    int quadIndex = 0;

    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;

        obj->vertex_buffer_data[i] = v < 2 ? -GLEXIT_SIZE : GLEXIT_SIZE;
        obj->vertex_buffer_data[i + 1] = -0.85f; //y
        obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? GLEXIT_SIZE : -GLEXIT_SIZE;

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
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLExit");
}

void gl_exit_free(GLExit *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

void gl_exit_draw(GLExit *obj, uint64_t time, t_vec3 gridPos) {
    t_vec3 worldPos;
    level_pos_to_world_pos3f(gridPos, worldPos);

    gl_tex_draw(&obj->gl_pt_object, worldPos, TEXTURE_EXIT);
}
