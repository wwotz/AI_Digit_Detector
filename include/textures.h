#ifndef TEXTURES_H_
#define TEXTURES_H_

#include "deps.h"

typedef enum TEXTURE_TYPES {
        TEXTURE_PIXELS, TEXTURE_FILE,
        TEXTURE_ENUM_COUNT
} TEXTURE_TYPES;

typedef struct texture_info_t {
        GLenum type;
        GLchar *file;
        GLuint *pixels;
        GLuint w, h;
} texture_info_t;

GLuint create_texture(texture_info_t info);
void delete_texture(GLuint texture);

#endif // TEXTURES_H_
