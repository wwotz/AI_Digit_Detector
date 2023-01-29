#include "../include/rect.h"

/* vertices for a rectangle
   specifying 2-components for the
   positional vertex and 2-components
   for the texture vertex */
static const GLfloat vertices[24] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
};

/* initialise a rect_t structure, returning 0 on success, -1 otherwise */
int init_rect(rect_t *rec, GLfloat x, GLfloat y, GLuint w, GLuint h,
              shader_info_t shaders[3])
{
        if (rec == NULL) {
                push_error_debug("Failed to initialise rect, was null!");
                return -1;
        }

        rec->x = x;
        rec->y = y;
        rec->w = w;
        rec->h = h;
        rec->program = load_program(shaders, 3);

        glGenVertexArrays(1, &rec->vao);
        glBindVertexArray(rec->vao);

        glGenBuffers(1, &rec->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, rec->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(*vertices) * 4, (GLvoid *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*vertices) * 4, (GLvoid *)(sizeof(*vertices) * 2));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        if (had_error_debug())
                return -1;
        return 0;
}

/* render a rect_t structure, ignoring if NULL */
void render_rect(rect_t *rec)
{
        if (rec != NULL) {
                identity_matrix(model);
                scale_matrix3f(model, rec->w, rec->h, 1.0);
                translate_matrix3f(model, rec->x, rec->y, 0.0);

                glUniformMatrix4fv(glGetUniformLocation(rec->program, "model"), 1, GL_FALSE, model);
                glUniformMatrix4fv(glGetUniformLocation(rec->program, "projection"), 1, GL_FALSE, projection);
                //glUniformMatrix4fv(glGetUniformLocation(rec->program, "view"), 1, GL_FALSE, view);

                glUseProgram(rec->program);
                glBindVertexArray(rec->vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
        }
}

/* free a rect_t structure, ignoring if NULL */
void free_rect(rect_t *rec)
{
        if (rec != NULL) {
                glDeleteProgram(rec->program);
                glDeleteBuffers(1, &rec->vbo);
                glDeleteVertexArrays(1, &rec->vao);

                rec->program = 0;
                rec->vbo = 0;
                rec->vao = 0;
        }
}
