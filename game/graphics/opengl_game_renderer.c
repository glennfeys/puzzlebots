#include "opengl_game_renderer.h"
#include "../../util/util.h"

#include <stdlib.h>

#include <SDL.h>

#undef main //Weird bug on windows where SDL overwrite main definition

#include <SDL_timer.h>
#include <assert.h>

#include <glmc.h>

static char* findGlErrorDescription(GLenum err) {
    switch(err) {
        case GL_INVALID_OPERATION: return "INVALID_OPERATION";
        case GL_INVALID_ENUM: return "INVALID_ENUM";
        case GL_INVALID_VALUE: return "INVALID_VALUE";
        case GL_OUT_OF_MEMORY: return "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
            // case GL_INVALID_INDEX: return "GL_INVALID_INDEX";
    }
    return "Unknown GL ERROR";
}

int handle_gl_error2(char *locationA, char *locationB) {
    char both[256];
    assert((strlen(locationA) + strlen(locationB) + 3) < 256);

    sprintf(both, "%s %s", locationA, locationB);
    both[255] = 0;

    return handle_gl_error(both);
}
int handle_gl_error(const char *location) {
    int hadError = 0;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        const char *description = findGlErrorDescription(err);
        if (description == NULL) {
            fprintf(stderr, "OpenGL error: %d", err);
        } else {
            fprintf(stderr, "OpenGL error: %d (%s)", err, description);
        }

        if (location != NULL)
            fprintf(stderr, " at \"%s\"\n", location);
        else
            fprintf(stderr, "\n");

        hadError = 1;
    }

    static int allowedErrors = 0;
    //make it fatal for debugging
    assert(!hadError || allowedErrors-- < 1);

    return hadError;
}

static GLchar* read_shader_code(char* filename) {
    FILE* fp = fopen (filename, "rb");
    if(!fp)
        fatal("Could not open '%s'. Are you in the right directory?\n", filename);

    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    if(file_size <= 0) {
        fclose(fp);
        fatal("file '%s' has size %d\n", filename, file_size);
    }
    rewind(fp);

    assert(sizeof(GLchar) == 1);
    GLchar* res = calloc(1, file_size+1);
    //note: the +1 assures that res is always null terminated
    if(!res) {
        fclose(fp);
        fatal("memory alloc failed allocating %d bytes for '%s'\n", file_size+1, filename);
    }

    if(fread(res, file_size, 1, fp) != 1) {
        free(res);
        fclose(fp);
        fatal("failed to read all %d bytes of '%s'\n", file_size, filename);
    }

    fclose(fp);

    return res;
}

static GLuint graphics_load_shaders(char* vertex_file_path, char* fragment_file_path){
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLchar* vertexShaderCode = read_shader_code(vertex_file_path);
    GLchar* fragmentShaderCode = read_shader_code(fragment_file_path);

    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile Vertex Shader
    glShaderSource(vertexShaderID, 1, (const GLchar **) &vertexShaderCode, NULL);
    free(vertexShaderCode);
    glCompileShader(vertexShaderID);

    handle_gl_error("glCompileShader()");

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        //GLchar vertexShaderErrorMessage[infoLogLength+1];
        GLchar * vertexShaderErrorMessage = (GLchar *)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, vertexShaderErrorMessage);
        fprintf(stdout, "Vertex shader compile message: '%s'\n", vertexShaderErrorMessage);
        free(vertexShaderErrorMessage);
    }
    if ( !result ){
        fatal("Failed to compile vertex shader. \n");
    }

    handle_gl_error("glGetShaderiv()");

    // Compile Fragment Shader
    glShaderSource(fragmentShaderID, 1, (const GLchar **) &fragmentShaderCode, NULL);
    free(fragmentShaderCode);
    glCompileShader(fragmentShaderID);

    handle_gl_error("glCompileShader()");

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ) {
        //GLchar fragmentShaderErrorMessage[infoLogLength + 1];
        GLchar * fragmentShaderErrorMessage = (GLchar *)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, fragmentShaderErrorMessage);
        fprintf(stdout, "Fragment shader compile message: '%s'\n", fragmentShaderErrorMessage);
        free(fragmentShaderErrorMessage);
    }
    if ( !result ){
        fatal("Failed to compile fragment shader. \n");
    }

    if (handle_gl_error("glGetShaderiv()")) {
        fatal("Error occured during GL Shader setup.");
    }

    // Link the program

    GLuint programID = glCreateProgram();

    if (handle_gl_error("glCreateProgram()") || programID == 0) {
        fatal("Error occured during GL Program Linking.");
    }

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    int sawError = handle_gl_error("glAttachShader()");

    if (sawError) {
        fatal("Error in LoadShaders");
    }

    glLinkProgram(programID);

    sawError |= handle_gl_error("glLinkProgram()");
    if (sawError) {
        fprintf(stderr, "Error when linking shader programs.\n");
        //the caller might try to fall back to another simpler shader
        //remove allocated memory but don't fail hard
        if (programID != 0) {
            glDeleteProgram(programID);
            handle_gl_error("glDeleteProgram");
        }
        return 0;
    }

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        fprintf(stderr, "Debug info: OpenGL Shading Language version: '%s'\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        //GLchar programErrorMessage[infoLogLength + 1];
        GLchar * programErrorMessage = (GLchar *)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetProgramInfoLog(programID, infoLogLength, NULL, programErrorMessage);
        fprintf(stdout, "Message when linking shader program. '%s'\n", programErrorMessage);
        free(programErrorMessage);
    }
    if (!result) {
        fprintf(stderr, "Failed to link GLSL program. \n");
        //the caller might try to fall back to another simpler shader
        //remove allocated memory but don't fail hard
        if (programID != 0) {
            glDeleteProgram(programID);
            handle_gl_error("glDeleteProgram");
        }
        return 0;
    }

    sawError |= handle_gl_error("glGetProgramiv()");

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    sawError |= handle_gl_error("glDetachShader()");

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    sawError |= handle_gl_error("glDeleteShader()");

    if (sawError) {
        fatal("Error in LoadShaders");
    }

    return programID;
}


int get_program_index(Graphics* g, GLuint program_id) {
    for (int i = 0; i < MAX_PROGRAM_COUNT; i++) {
        if (g->program_id[i] == program_id)
            return i;
    }
    return 0;
}
GLuint get_program_id(Graphics* g, int program_index) {
    return g->program_id[program_index];
}


static GLuint init_attrib_location(GLuint program, char* attrib_name) {
    GLuint attribute_id = glGetAttribLocation(program, attrib_name);
    if (handle_gl_error("glGetAttribLocation()")) {
        fatal("Error in glGetAttribLocation for program %d attribute %d (\"%s\")", program, attribute_id, attrib_name);
    }
    return attribute_id;
}

static GLuint init_uniform_location(GLuint program_id, char* uniform_name) {
    GLuint uniform_id = glGetUniformLocation(program_id, uniform_name);
    if (handle_gl_error("glGetUniformLocation()")) {
        fatal("Error in glGetUniformLocation for program %d uniform %d (\"%s\")", program_id, uniform_id, uniform_name);
    }
//    printf("Program %d Uniform %s ID = %d\n", program_id, uniform_name, uniform_id);
    return uniform_id;
}



static void init_attributes_and_uniform_ids(Graphics *graphics, int program_index, GLuint program_id) {
    assert(graphics != NULL);
    assert(program_id != 0);
    assert(graphics->program_id[program_index] == program_id);

#define X(ATTRIB) graphics->attribute_##ATTRIB[program_index] = init_attrib_location(program_id, #ATTRIB);
    ATTRIBUTE_XMACROS
#undef X

#define X(UNIFORM) graphics->uniform_##UNIFORM[program_index] = init_uniform_location(program_id, #UNIFORM);
    UNIFORM_XMACROS
#undef X
}


//t_vec3 levelPosToWorldPos2i(t_ivec2 levelPos) {
//return glm::vec3(
//(2.0f * levelPos.x),
//(0.0f),
//(-2.0f * levelPos.y)
//);
//}
//t_vec3 levelPosToWorldPos3i(t_ivec3 levelPos) {
//return glm::vec3(
//(2.0f * levelPos.x),
//(2.0f * levelPos.z),
//(-2.0f * levelPos.y)
//);
//}
void level_pos_to_world_pos3i(t_ivec3 levelPos, t_vec3 dest) {
    assert((void*)levelPos != (void*)dest);
    dest[0] = 2.0f * levelPos[0];
    dest[1] = 2.0f * levelPos[2];
    dest[2] = -2.0f * levelPos[1];
}
void level_pos_to_world_pos3f(t_vec3 levelPos, t_vec3 dest) {
    assert(levelPos != dest);
    dest[0] = 2.0f * levelPos[0];
    dest[1] = 2.0f * levelPos[2];
    dest[2] = -2.0f * levelPos[1];
}

int is_sensible_world_coordinate(t_vec3 worldPos) {
    if (worldPos[0] < -50.0) return 0;
    if (worldPos[1] < -50.0) return 0;
    if (worldPos[2] < -50.0) return 0;
    if (worldPos[0] > 50.0) return 0;
    if (worldPos[1] > 50.0) return 0;
    if (worldPos[2] > 50.0) return 0;
    return 1;
}


int is_sensible_color(t_vec4 color) {
    //special colors interpreted by fragment shader
    if (color[0] == 2.0f) return 1;
    if (color[0] == 3.0f) return 1;
    if (color[0] == 4.0f) return 1;
    if (color[0] == 5.0f) return 1;
    if (color[0] == 6.0f) return 1;

    for (int i = 0; i < 4; i++) {
        if (color[i] < 0.0f) return 0;
        if (color[i] > 1.0f) return 0;
    }
    return 1;
}


void gl_copy_color_to_from(gl_color dest, gl_color source) {
    dest[0] = source[0];
    dest[1] = source[1];
    dest[2] = source[2];
    dest[3] = source[3];
}

void gl_set_color(gl_color dest, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    dest[0] = r;
    dest[1] = g;
    dest[2] = b;
    dest[3] = a;
}


Graphics* graphics_alloc(int width, int height) {
    Graphics* res = malloc(sizeof(Graphics));
    graphics_init(res, width, height);
    return res;
}

void graphics_clear(Graphics* g) {

}

void graphics_free(Graphics* g) {
    glDeleteProgram(g->program_id[PROGRAM_GAME]);
    handle_gl_error("glDeleteProgram");
    g->program_id[PROGRAM_GAME] = -1;


    SDL_Quit( );
}

void graphics_init(Graphics* graphics, int width, int height) {
    memset(graphics, 0, sizeof(Graphics));

    for (int i = 0; i < MAX_PROGRAM_COUNT; i++)
        graphics->program_id[i] = 0;

    graphics->background_color[0] = 0;
    graphics->background_color[1] = 0;
    graphics->background_color[2] = 0.3f;

    graphics->width = width;
    graphics->height = width;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fatal("Video initialization failed: %s\n", SDL_GetError());
    }

    /* Information about the current video settings. */
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    if (!info) {
        fatal("Video query failed: %s\n", SDL_GetError());
    }

    if (graphics->width <= 0 || graphics->height <= 0) {
        int w = info->current_w > 1920 ? 1920 : info->current_w;
        int h = info->current_h > 1080 ? 1080 : info->current_h;
        graphics->width = w;
        graphics->height = h;
        graphics->width = w;
        graphics->height = h;
        printf("Using automatic resolution: %dx%d\n", graphics->width, graphics->height);
    }

    graphics->screenMidX = graphics->width / 2;
    graphics->screenMidY = graphics->height / 2;

    /* Color depth in bits of our window. */
    int bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

//    flags = SDL_OPENGL  | SDL_FULLSCREEN;
    int flags = SDL_OPENGL;
    graphics->vidSurface = SDL_SetVideoMode(graphics->width, graphics->height, bpp, flags);
    if (graphics->vidSurface == NULL) {
        fatal("Video mode set failed: %s\n", SDL_GetError());
    }

    // Initialize GLEW
    glewExperimental = 1; // Needed for core profile
    GLenum glewInitErr = glewInit();
    if (glewInitErr != GLEW_OK) {
        fatal("Failed to initialize GLEW: \"%s\"\n", glewGetErrorString(glewInitErr));
    }

    if (!GLEW_VERSION_2_1) {  // check that the machine supports the 2.1 API.
        fatal("GLEW is not using the 2.1 API. You need at least OpenGL ES 2.0.");
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    int r, g, b, d;
    SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r);
    SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g);
    SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &d);
    //printf("Window has OpenGL context with sizes: Red=%d, Green=%d, Blue=%d Depth=%d\n", r, g, b, d);



    SDL_ShowCursor(1);

    //window is ready, now setup OpenGL

    // Create and compile our GLSL program from the shaders
    graphics->program_id[PROGRAM_GAME] = graphics_load_shaders("game/graphics/game.vertexshader",
                                                                  "game/graphics/game.fragmentshader");
    if (graphics->program_id[PROGRAM_GAME] == 0) {
        fprintf(stderr, "Loading shaders failed, trying the RPI one.\n");
        graphics->program_id[PROGRAM_GAME] = graphics_load_shaders("game/graphics/game.pi.vertexshader",
                                                                "game/graphics/game.pi.fragmentshader");
        if (graphics->program_id[PROGRAM_GAME] == 0) {
            fatal("Something went wrong with shader. Cannot continue.\n");
        }
    }

    printf("Program %d ProcTex ID=%d\n", PROGRAM_GAME, graphics->program_id[PROGRAM_GAME]);
    init_attributes_and_uniform_ids(graphics, PROGRAM_GAME, graphics->program_id[PROGRAM_GAME]);

    char *font_image_filenames[TEXTURECOUNT] = {NULL};
    font_image_filenames[TEXTURE_WALL] = "textures/wall.png";
    font_image_filenames[TEXTURE_SHADE_WALL] = "textures/wall_shade.png";
    font_image_filenames[TEXTURE_FLOOR] = "textures/floor.png";
    font_image_filenames[TEXTURE_SHADE_FLOOR] = "textures/floor_shade.png";
    font_image_filenames[TEXTURE_FLOOR_ACTIVE] = "textures/floor_active.png";
    font_image_filenames[TEXTURE_DOOR_A] = "textures/door_a.png";
    font_image_filenames[TEXTURE_DOOR_B] = "textures/door_b.png";
    font_image_filenames[TEXTURE_DOOR_C] = "textures/door_c.png";
    font_image_filenames[TEXTURE_BUTTON_ONCE_A] =   "textures/button_once_a.png";
    font_image_filenames[TEXTURE_BUTTON_ONCE_B] =   "textures/button_once_b.png";
    font_image_filenames[TEXTURE_BUTTON_ONCE_C] =   "textures/button_once_c.png";
    font_image_filenames[TEXTURE_BUTTON_HOLD_A] =   "textures/button_hold_a.png";
    font_image_filenames[TEXTURE_BUTTON_HOLD_B] =   "textures/button_hold_b.png";
    font_image_filenames[TEXTURE_BUTTON_HOLD_C] =   "textures/button_hold_c.png";
    font_image_filenames[TEXTURE_BUTTON_ACTIVE_A] = "textures/button_active_a.png";
    font_image_filenames[TEXTURE_BUTTON_ACTIVE_B] = "textures/button_active_b.png";
    font_image_filenames[TEXTURE_BUTTON_ACTIVE_C] = "textures/button_active_c.png";
    font_image_filenames[TEXTURE_EYE] = "textures/eye.png";
    font_image_filenames[TEXTURE_EYE_CONTROLLED] = "textures/eye_controlled.png";
    font_image_filenames[TEXTURE_BOT] = "textures/bot.png";
    font_image_filenames[TEXTURE_BOT_CONTROLLED] = "textures/bot_controlled.png";
    font_image_filenames[TEXTURE_TELEPORT_A] = "textures/teleport_green.png";
    font_image_filenames[TEXTURE_TELEPORT_B] = "textures/teleport_purple.png";
    font_image_filenames[TEXTURE_TELEPORT_C] = "textures/teleport_yellow.png";
    font_image_filenames[TEXTURE_TELEPORT_D] = "textures/teleport_pink.png";
    font_image_filenames[TEXTURE_TELEPORT_E] = "textures/teleport_blue.png";
    font_image_filenames[TEXTURE_EXIT] = "textures/exit.png";
    font_image_filenames[TEXTURE_HEXPRISM] = "textures/hexprism.png";
    font_image_filenames[TEXTURE_TETRAHEDRON] = "textures/tetrahedron.png";

    for (int tex_id = 0; tex_id < TEXTURECOUNT; tex_id++) {
        char *font_image_filename = font_image_filenames[tex_id];
        if (font_image_filename == NULL)
            font_image_filename = "textures/unknown.png";

        SDL_Surface *loadedSurface = IMG_Load(font_image_filename);
        if (loadedSurface == NULL) {
            fatal("Unable to load image %s! SDL_image Error: %s\n", font_image_filename, IMG_GetError());
        }

        //not using blending, so no need for SDL_DisplayFormatAlpha: to do blending right,
        //  we'd have to order the draws by farest from camera first. And we won't do that.
//        graphics->texture_surfaces[tex_id] = SDL_DisplayFormatAlpha(loadedSurface);
//        if (graphics->texture_surfaces[tex_id] == NULL) {
//            fatal("Unable to optimize image %s! SDL Error: %s\n", font_image_filename, SDL_GetError());
//        }
//        //Get rid of old loaded surface
//        SDL_FreeSurface(loadedSurface);
        graphics->texture_surfaces[tex_id] = loadedSurface;

        int w = graphics->texture_surfaces[tex_id]->w;
        int h = graphics->texture_surfaces[tex_id]->h;

        handle_gl_error("PRE gl_glyph init glBindTexture");

        glGenTextures(1, &graphics->texture_ids[tex_id]);
        glBindTexture(GL_TEXTURE_2D, graphics->texture_ids[tex_id]);
        handle_gl_error("POST gl_glyph init glBindTexture");


        int max_texture_size = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
//        printf("Loaded image with size %dx%d  (max is %d)\n", w, h, max_texture_size);

        //not using blending, so no alpha needed: to do blending right,
        //  we'd have to order the draws by farest from camera first. And we won't do that.
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,     //without alpha
//                     GL_RGBA8,  //with alpha
                     w,
                     h,
                     0,
                     GL_RGB,      //without alpha
//                     GL_BGRA,   //with alpha
                     GL_UNSIGNED_BYTE,
                     graphics->texture_surfaces[tex_id]->pixels);
        handle_gl_error("POST gl_glyph init glTexImage2D");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST

        handle_gl_error("POST gl_glyph init");
    }
}

void mat4_multiply(t_mat4 a, t_mat4 b, t_mat4 dest) {
    dest[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
    dest[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
    dest[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
    dest[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];

    dest[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
    dest[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
    dest[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
    dest[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];

    dest[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
    dest[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
    dest[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
    dest[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];

    dest[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
    dest[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
    dest[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
    dest[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

void graphics_begin_draw(Graphics* graphics) {
    //draw without light

    t_vec3 dummy = { 0.0f };
    graphics_begin_draw_withlight(graphics, 0.0f, dummy);
}

void graphics_begin_draw_withlight(Graphics* graphics, float lightPower, t_vec3 lightPos) {
    handle_gl_error("PRE graphics_begin_draw_withlight");


    //TODO: implement glm calculations in C
    //a good start is GLM C -> https://github.com/prdbrg/glmc

//    t_vec4 frustrum = { 45.0f, 4.0f / 3.0f, 0.1f, 100.0f };
    //reduced far and near plane to reduce z-fighting
    t_vec4 frustrum = { 45.0f, 4.0f / 3.0f, 2.0f, 60.0f };
    t_mat4 projection;// = { 0 };
    glmc_perspective(frustrum, projection);

    t_vec3 lookFrom, lookAt;
    level_pos_to_world_pos3f(graphics->cameraLookFrom, lookFrom);
    level_pos_to_world_pos3f(graphics->cameraLookAt, lookAt);

    t_mat4 view;// = { 0 };
    glmc_look_at(lookFrom, lookAt, graphics->cameraUp, view);

    t_mat4 model;// = { 0 };
    glmc_identity(model);

    t_mat4 vp;// = { 0 };
    mat4_multiply(projection, view, vp);
    t_mat4 mvp;// = { 0 };
    mat4_multiply(vp, model, mvp);


    // Clear the screen
    glUseProgram(graphics->program_id[PROGRAM_GAME]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_gl_error("graphics_begin_draw_withlight glClear");

    glClearColor(graphics->background_color[0], graphics->background_color[1], graphics->background_color[2], 1.0f);



    handle_gl_error("graphics_begin_draw_withlight pre glUniform handling");
//    for (int i = 0 ; i < PROGRAM_COUNT; i++) {
//        ProgramSpecificInfo* program = programs[i];

    glUseProgram(graphics->program_id[PROGRAM_GAME]);

    //***** Uniforms the same for entire scene **********
    if (HAS_UNIFORM(graphics, PROGRAM_GAME, MVP)) {
        glUniformMatrix4fv(graphics->uniform_MVP[PROGRAM_GAME], 1, GL_FALSE, mvp);
        if (handle_gl_error("graphics_begin_draw_withlight glUniformMatrix4fv() MVP")) {
            fprintf(stderr, "        FAILED glUniformMatrix4fv() MVP for programId=%d  MVPid=%u\n",
                    graphics->program_id[PROGRAM_GAME],
                    graphics->uniform_MVP[PROGRAM_GAME]);
        }
    }
    if (HAS_UNIFORM(graphics, PROGRAM_GAME, M)) {
        glUniformMatrix4fv(graphics->uniform_M[PROGRAM_GAME], 1, GL_FALSE, model);
        handle_gl_error("graphics_begin_draw_withlight glUniformMatrix4fv() M");
    }
    if (HAS_UNIFORM(graphics, PROGRAM_GAME, V)) {
        glUniformMatrix4fv(graphics->uniform_V[PROGRAM_GAME], 1, GL_FALSE, view);
        handle_gl_error("graphics_begin_draw_withlight glUniformMatrix4fv() V");
    }

    if (HAS_UNIFORM(graphics, PROGRAM_GAME, LightPosition_worldspace)) {
        assert(is_sensible_world_coordinate(lightPos));
        glUniform3f(graphics->uniform_LightPosition_worldspace[PROGRAM_GAME], lightPos[0], lightPos[1], lightPos[2]);
        handle_gl_error("graphics_begin_draw_withlight glUniform3f() LightPosition_worldspace");
    }

    if (HAS_UNIFORM(graphics, PROGRAM_GAME, TimeTicks)) {
        glUniform1f(graphics->uniform_TimeTicks[PROGRAM_GAME], SDL_GetTicks() / 10.0f);
        handle_gl_error("graphics_begin_draw_withlight glUniform1f() TimeTicks");
    }

    //defaults in case any object forgets to set them
//    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
//    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    }
}

void graphics_end_draw(Graphics* g) {
/*
 * Swap the buffers. This this tells the driver to
 * render the next frame from the contents of the
 * back-buffer, and to set all rendering operations
 * to occur on what was the front-buffer.
 *
 * Double buffering prevents nasty visual tearing
 * from the application drawing on areas of the
 * screen that are being updated at the same time.
 */

    SDL_GL_SwapBuffers( );

    handle_gl_error("POST graphics_end_draw()");
}
