#ifndef SHADERS_H_
#define SHADERS_H_

#include "deps.h"
#include "debug.h"

typedef struct shader_info_t {
        GLenum type;
        GLchar *file;
} shader_info_t;

GLuint load_shader(shader_info_t info);
GLuint load_program(shader_info_t *info, size_t size);

#endif // SHADERS_H_
