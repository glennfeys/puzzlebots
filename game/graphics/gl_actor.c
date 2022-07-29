#include <assert.h>
#include "gl_actor.h"
#include <math.h>

void gl_actor_init(GLActor *obj, Graphics *graphics, float scale) {
    gl_tex_object_init(&obj->gl_pt_object, graphics, PROGRAM_GAME);
    obj->scale = scale;

    int indicesIndex = 0;

    for (int side = 0; side < 6; side++) {
        int normal_x = side == 0 ? 1 : ( side == 1 ? -1 : 0);
        int normal_y = side == 2 ? 1 : ( side == 3 ? -1 : 0);
        int normal_z = side == 4 ? 1 : ( side == 5 ? -1 : 0);

        float tex_x_start = 0.0;
        float tex_y_start = 0.0;
        switch (side) {
            case 0: { //side 1
                tex_x_start = 0.0; tex_y_start = 0.5;
                break;
            }
            case 1: { //side 2
                tex_x_start = 0.0; tex_y_start = 0.5;
                break;
            }
            case 2: { //top
                tex_x_start = 0.0; tex_y_start = 0.0;
                break;
            }
            case 3: { //bottom
                tex_x_start = 0.0; tex_y_start = 0.0;
                break;
            }
            case 4: { //back
                tex_x_start = 0.5; tex_y_start = 0.5;
                break;
            }
            case 5: { //front
                tex_x_start = 0.5; tex_y_start = 0.0;
                break;
            }
        }
        float tex_x_stop = tex_x_start + 0.5;
        float tex_y_stop = tex_y_start + 0.5;

        for (int v = 0; v < 4; v++) {
            int i = ((side * 4) + v) * 3;

            obj->vertex_buffer_data[i] = normal_x == 0 ?
                                         (v < 2 ? -scale : scale) :
                                         normal_x * scale; //x
            obj->vertex_buffer_data[i + 1] = normal_y == 0 ?
                                             (normal_x != 0 ? (v < 2 ? -scale : scale) : (v % 2 == 0 ? scale : -scale)) :
                                             normal_y * scale; //y
            obj->vertex_buffer_data[i + 2] = normal_z == 0 ?
                                             (v % 2 == 0 ? scale : -scale) :
                                             normal_z * scale; //z

            obj->normal_buffer_data[i] = normal_x * 1.0; //x
            obj->normal_buffer_data[i + 1] = normal_y * 1.0; //y
            obj->normal_buffer_data[i + 2] = normal_z * 1.0; //z

            int j = ((side * 4) + v) * 2;
            if (side < 2) {
                obj->texCoord_buffer_data[j] = v % 2 == 0 ? tex_x_start : tex_x_stop; //u
                obj->texCoord_buffer_data[j + 1] =  v < 2 ? tex_y_stop : tex_y_start; //w
            } else {
                obj->texCoord_buffer_data[j] = v < 2 ? tex_x_start : tex_x_stop; //u
                obj->texCoord_buffer_data[j + 1] = v % 2 == 0 ? tex_y_start : tex_y_stop; //w
            }
        }

        if (side != 1 && side != 2 && side != 5) {
            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 1;
            obj->indices[indicesIndex++] = (side * 4) + 3;

            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 3;
            obj->indices[indicesIndex++] = (side * 4) + 2;
        } else {
            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 3;
            obj->indices[indicesIndex++] = (side * 4) + 1;

            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 2;
            obj->indices[indicesIndex++] = (side * 4) + 3;
        }
    }

    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_tex_init_vertexbuffers(&obj->gl_pt_object, 6 * 4, "GLActor");
}

void gl_actor_free(GLActor *obj) {
    gl_tex_object_free(&obj->gl_pt_object);
}

static void get_world_pos(uint64_t time, t_vec3 gridPos, t_vec3 dest, float scale, int unique_id) {
    float bound_x = 0.20f;
    float bound_y = 0.20f;
    float bound_z = 0.30f;
    float base_x = (time / (1000.0f+(sinf((unique_id * 874.0f)) * 700.0f)));
    float base_y = (time / (1300.0f+(sinf((unique_id * 691.0f)) * 300.0f)));
    float base_z = (time / (1400.0f+(sinf((unique_id * 431.0f)) * 500.0f)));
    float multi_x = cosf(base_x) * bound_x;
    float multi_y = sinf(base_y) * bound_y;
    float multi_z = ((sinf(base_z) * bound_z) + 1.0f) / 0.5f; //[0.0, 1.0f]

//    dest[0] = dest[0] + sinf(base) * bound;
//    dest[1] = dest[1] + cosf(base) * bound;
//    dest[2] = dest[2] + sinf(base*2.5f) * bound;
    level_pos_to_world_pos3f(gridPos, dest);
    dest[0] = dest[0] - (multi_x * bound_x);
//    dest[1] = dest[1] - (1.0f - scale); //drop player on floor
    dest[1] = dest[1] + (scale * multi_z * bound_z) - (1.0f - scale); //hover player above floor
    dest[2] = dest[2] - (multi_y * bound_y);
}

void gl_actor_draw(GLActor *obj, uint64_t time, t_vec3 gridPos, float rotation_angle, int bot, int controlled, int unique_id) {
    t_vec3 worldPos;
    get_world_pos(time, gridPos, worldPos, obj->scale, unique_id);

    gl_tex_draw_rotated(&obj->gl_pt_object, worldPos, rotation_angle,
                            bot ?
                            (controlled ? TEXTURE_BOT_CONTROLLED : TEXTURE_BOT) :
                            (controlled ? TEXTURE_EYE_CONTROLLED : TEXTURE_EYE));
}
