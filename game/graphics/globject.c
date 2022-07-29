#include <assert.h>
#include <string.h>
#include "globject.h"
#include "../../util/util.h"

void globject_init(GLObject* obj, Graphics* graphics, GLuint program_index) {
    obj->program_index = program_index;
    obj->elementbuffer = -1;
    obj->indicesCount = 0;
    
    assert(graphics != NULL);
    assert(graphics->program_id[program_index] != 0);
    
    assert(sizeof(obj->indices) / sizeof(unsigned short) == GLOBJECT_MAX_INDICES);
    
    memset(&obj->indices[0], -1, sizeof(obj->indices));
    
    glGenBuffers(1, &obj->elementbuffer);
    handle_gl_error2("GLObject", "glGenBuffers() for indices");
}

void globject_free(GLObject *obj) {
    //nothing to do
}

void globject_init_indices(GLObject *obj, Graphics *g, int indicesCount) {
    obj->indicesCount = indicesCount;
    assert((unsigned int)indicesCount <= (sizeof(obj->indices) / sizeof(unsigned short)));
    assert(indicesCount > 0);
    
    for (int i = 0; i < indicesCount; i++) {
        assert(indicesCount > 0);
        assert(indicesCount < GLOBJECT_MAX_INDICES);
        if (obj->indices[i] < 0) {
            fatal("Invalid indices[%d] = %d\n", i, obj->indices[i]);
        }
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short int) * indicesCount, obj->indices, GL_STATIC_DRAW);
    handle_gl_error2("GLObject", "glBindBuffer() glBufferData() for indices");
}
