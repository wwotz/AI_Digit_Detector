#ifndef RECT_H_
#define RECT_H_

#include "shaders.h"
#include "debug.h"

/* structure containing:
   - (v)ertex (b)uffer (o)bject,
   - (v)ertex (a)rray (o)bject
   - program (containing vertex and fragment shader)
   - texture (optional texture to be bound)
*/

typedef struct rect_t {
        GLfloat x, y;
        GLuint w, h;
        GLuint vbo, vao, program, texture;
} rect_t;

int init_rect(rect_t *rec, GLfloat x, GLfloat y, GLuint w, GLuint h,
              shader_info_t shaders[3]);
void render_rect(rect_t *rec);
void free_rect(rect_t *rec);

#endif // RECT_H_
