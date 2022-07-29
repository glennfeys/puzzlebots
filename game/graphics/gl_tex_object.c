#include "gl_tex_object.h"

#include <assert.h>
#include <string.h>
#include "globject.h"
#include "../../util/util.h"

void gl_tex_object_init(GLTexObject *obj, Graphics *graphics, int program_index) {
    globject_init(&obj->gl_object, graphics, program_index);
    
    obj->graphics = graphics;
    obj->program_index = program_index;
    obj->program_id = get_program_id(graphics, program_index);
    
    obj->blend = 0;
    obj->no_cull = 0;
    
    memset(&obj->vertex_buffer_data[0], -1000, sizeof(obj->vertex_buffer_data));
    memset(&obj->normal_buffer_data[0], -1000, sizeof(obj->normal_buffer_data));
    memset(&obj->texCoord_buffer_data[0], -1000, sizeof(obj->texCoord_buffer_data));
    
    assert(HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexBasePosition_modelspace));
//    assert(HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace));
//    assert(HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexTexCoord));
    
    glGenBuffers(1, &obj->vertexbuffer);
    glGenBuffers(1, &obj->normalbuffer);
    glGenBuffers(1, &obj->texcoordbuffer);
    handle_gl_error2("GLTexObject", "glGenBuffers()");
}

void gl_tex_object_free(GLTexObject *obj) {
    glDeleteBuffers(1, &obj->vertexbuffer);
    handle_gl_error2("GLTexObject", "glDeleteBuffers() for VertexBasePosition_modelspace");
    
    glDeleteBuffers(1, &obj->normalbuffer);
    handle_gl_error2("GLTexObject", "glDeleteBuffers() for VertexNormal_modelspace");
    
    glDeleteBuffers(1, &obj->texcoordbuffer);
    handle_gl_error2("GLTexObject", "glDeleteBuffers() for VertexTexCoord");
    
    globject_free(&obj->gl_object);
}

void gl_tex_init_vertexbuffers(GLTexObject *obj, int vertexCount, char *objName) {
    obj->vertexCount = vertexCount;
    assert(vertexCount <= GLLEVELPART_MAX_VERTICES);
    assert(vertexCount > 0);
    
    for (int i = 0; i < vertexCount * 3; i++) {
        GLfloat vertex = obj->vertex_buffer_data[i];
        //we expect all models to be max distance 1 from source
        if (vertex < -1.0 || vertex > 1.0) {
            fatal("Check failed: %s index=%d vertex=%f", objName, i, vertex);
        }
        
        //normals should be normalized
        GLfloat normal = obj->normal_buffer_data[i];
        if (normal < -1.0 || normal > 1.0) {
            fatal("Check failed: %s index=%d normal=%f", objName, i, normal);
        }
    }
    for (int i = 0; i < vertexCount * 2; i++) {
        GLfloat texCoord = obj->texCoord_buffer_data[i];
        //texture coordinates are always between 0.0 and 1.0
        if (texCoord < 0.0 || texCoord > 1.0) {
            fatal("Check failed: %s index=%d texCoord=%f", objName, i, texCoord);
        }
    }
    //indices should already be set, so we check if they all point to an existing vertex
    for (int i = 0; i < obj->gl_object.indicesCount; i++) {
        assert(obj->gl_object.indicesCount > 0);
        assert(obj->gl_object.indicesCount < GLOBJECT_MAX_INDICES);
        if (obj->gl_object.indices[i] < 0 || obj->gl_object.indices[i] >= vertexCount) {
            fatal("Invalid indices[%d] = %d while vertexCount=%d\n", i, obj->gl_object.indices[i], vertexCount);
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * vertexCount, obj->vertex_buffer_data, GL_STATIC_DRAW);
    handle_gl_error2(objName, "glGenBuffers() glBindBuffer() glBufferData() for VertexBasePosition_modelspace");
    
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace)) {
        glBindBuffer(GL_ARRAY_BUFFER, obj->normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * vertexCount, obj->normal_buffer_data, GL_STATIC_DRAW);
        handle_gl_error2(objName,
                         "glGenBuffers() glBindBuffer() glBufferData() for VertexNormal_modelspace");
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->texcoordbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * vertexCount, obj->texCoord_buffer_data, GL_STATIC_DRAW);
    handle_gl_error2(objName, "glGenBuffers() glBindBuffer() glBufferData() for VertexTexCoord");
}


void gl_tex_draw_call(GLTexObject *obj) {
    //not using blending: to do it right, we'd have to order the draws by farest from camera first. And we won't do that.
//    if (obj->blend) {
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    }
    
    if (obj->no_cull) {
        glCullFace(GL_FRONT);
    }

    //no fuzzy images, we want to see pixels
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//    printf("Draw vertexbuffer=%d normalbuffer=%d texbuffer=%d elementbuffer=%d\n", vertexbuffer, normalbuffer, texbuffer, elementbuffer);
//    printf("Draw vertexCount=%d indicesCount=%d\n", vertexCount, indicesCount);
    glDrawElements(
            GL_TRIANGLES,      // mode
            obj->gl_object.indicesCount,   // count
            GL_UNSIGNED_SHORT,   // type
            (void*)0           // element array buffer offset
    );
    
    if (obj->no_cull) {
        glCullFace(GL_BACK);
        glDrawElements(
                GL_TRIANGLES,      // mode
                obj->gl_object.indicesCount,   // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0           // element array buffer offset
        );
    }
    
//    if (obj->blend) {
//        glDisable(GL_BLEND);
//    }
}

void gl_tex_draw(GLTexObject* obj, t_vec3 translation, t_texid texid) {
    gl_tex_draw_rotated(obj, translation, 0.0f, texid);
}

void gl_tex_draw_rotated(GLTexObject* obj, t_vec3 translation, float rotation_angle, t_texid texid) {
    //check if init done correctly
    assert(obj->gl_object.elementbuffer >= 0);
    assert(obj->gl_object.indicesCount > 0);
    assert(obj->gl_object.indicesCount < GLOBJECT_MAX_INDICES);
    assert(obj->vertexbuffer >= 0);
    assert(obj->vertexCount > 0);
    assert(obj->vertexCount <= GLOBJECT_MAX_VERTICES);
    
    glUseProgram(obj->program_id);

    handle_gl_error("PRE gl_tex_draw tex");
    glBindTexture(GL_TEXTURE_2D, obj->graphics->texture_ids[texid]);
    handle_gl_error("POST gl_tex_draw tex");

    GLint vertexPosID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexBasePosition_modelspace);
    glEnableVertexAttribArray(vertexPosID);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexbuffer);
    glVertexAttribPointer(
            vertexPosID,                       // attribute
            3,                                    // size
            GL_FLOAT,                             // type
            GL_FALSE,                             // normalized?
            0,                                    // stride
            (void *) 0                              // array buffer offset
    );
    handle_gl_error("GLTexObject glEnableVertexAttribArray(vertexPosID)");
    
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace)) {
        GLint normalID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexNormal_modelspace);
        glEnableVertexAttribArray(normalID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->normalbuffer);
        glVertexAttribPointer(
                normalID,                      // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                        // array buffer offset
        );
        handle_gl_error("GLTexObject glEnableVertexAttribArray(normalID)");
    }
    
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexTexCoord)) {
        GLint texCoordID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexTexCoord);
        glEnableVertexAttribArray(texCoordID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->texcoordbuffer);
        glVertexAttribPointer(
                texCoordID,                    // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                        // array buffer offset
        );
        handle_gl_error("GLTexObject glVertexAttribPointer(texbuffer)");
    }
    
    
    //set uniforms specific for this object
    if (HAS_UNIFORM(obj->graphics, obj->program_index, PerDrawTransformation_worldspace)) {
        t_mat4 iden4;
        glmc_identity(iden4);
        
        t_mat4 tmp;
        glmc_translate(iden4, translation, tmp);
    
        t_mat4 preTransformationMatrix;
        t_mat4* final;
        if (rotation_angle != 0.0f) {
            t_vec3 axis = {0.0f, 1.0f, 0.0f};
            glmc_rotate(tmp, rotation_angle, axis, preTransformationMatrix);
            final = &preTransformationMatrix;
        } else {
            final = &tmp;
        }
        
        glUniformMatrix4fv(GET_UNIFORM_ID(obj->graphics, obj->program_index, PerDrawTransformation_worldspace), 1, GL_FALSE, *final);
        handle_gl_error("GLTexObject::draw() glUniformMatrix4fv() PerDrawTransformation_worldspace");
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->gl_object.elementbuffer);
    gl_tex_draw_call(obj);
    handle_gl_error("GLTexObject glDrawElements()");
    
    glDisableVertexAttribArray(vertexPosID);
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexNormal_modelspace)) {
        GLint normalID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexNormal_modelspace);
        glDisableVertexAttribArray(normalID);
    }
    if (HAS_ATTRIBUTE(obj->graphics, obj->program_index, VertexTexCoord)) {
        GLint texCoordID = GET_ATTRIBUTE_ID(obj->graphics, obj->program_index, VertexTexCoord);
        glDisableVertexAttribArray(texCoordID);
    }
}
