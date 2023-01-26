#include "../include/shaders.h"

/* return if @shader had an error compiling, if the
   shader is not a shader, then there was an error */
static GLboolean had_error_shader(GLuint shader)
{
        if (glIsShader(shader)) {
                GLint status;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
                return !status;
        }

        return GL_TRUE;
}

/* return if the program had an error linking, if
   @program is not a program, then there was an error */
static GLboolean had_error_program(GLuint program)
{
        if (glIsProgram(program)) {
                GLint status;
                glGetProgramiv(program, GL_LINK_STATUS, &status);
                return !status;
        }

        return GL_TRUE;
}

/* returns a malloc'd buffer containing error information
   about @shader */
static char *get_error_shader(GLuint shader)
{
        if (glIsShader(shader)) {
                char *msg;
                GLint msg_len, max_len;

                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
                msg = malloc(max_len);
                glGetShaderInfoLog(shader, max_len, &msg_len, msg);
                if (msg_len > 0)
                        return msg;
                free(msg);
                return NULL;
        }

        return NULL;
}

/* returns a malloc'd buffer containing error information
   about @program */
static char *get_error_program(GLuint program)
{
        if (glIsProgram(program)) {
                char *msg;
                GLint msg_len, max_len;

                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_len);
                msg = malloc(max_len);
                glGetProgramInfoLog(program, max_len, &msg_len, msg);
                if (msg_len > 0)
                        return msg;
                free(msg);
                return NULL;
        }

        return NULL;
}

/* load a shader of specified @type from file @name,
   returns non-zero on success representing the number
   assocated with the respective compiled shader. */
GLuint load_shader(shader_info_t info)
{
        GLuint shader;
        GLchar *buffer;

        if (!info.file) {
                push_error_debug("Invalid File Name");
                return 0;
        }

        shader = glCreateShader(info.type);
        if (!shader) {
                push_error_debug("Invalid Shader Type");
                return 0;
        }

        buffer = load_file(info.file);
        if (!buffer) {
                glDeleteShader(shader);
                return 0;
        }

        glShaderSource(shader, 1, (const GLchar **)&buffer, NULL);
        glCompileShader(shader);
        return shader;
}

/* finds the first vertex, fragment and geometry shader,
   in @info, attaches them to a program, and returns
   a non-zero number representing the program */
GLuint load_program(shader_info_t *info, size_t size)
{
        GLuint program;
        GLboolean have_vertex, have_fragment, have_geometry;

        program = glCreateProgram();
        if (!program) {
                push_error_debug("Failed to create program object!");
                return 0;
        }

        have_vertex = have_fragment = have_geometry = GL_FALSE;
        for (int i = 0; i < size && ((!have_vertex)
                     || (!have_fragment) || (!have_geometry)); i++) {
                GLuint shader;
                shader_info_t curr_info = info[i];

                if (curr_info.type == GL_VERTEX_SHADER && !have_vertex) {
                        have_vertex = GL_TRUE;
                } else if (curr_info.type == GL_FRAGMENT_SHADER && !have_fragment) {
                        have_fragment = GL_TRUE;
                } else if (curr_info.type == GL_GEOMETRY_SHADER && !have_geometry) {
                        have_geometry = GL_TRUE;
                } else {
                        continue;
                }

                shader = load_shader(curr_info);
                if (had_error_shader(shader)) {
                        char *msg = get_error_shader(shader);
                        push_error_debug(msg);
                        free(msg);
                        glDeleteProgram(program);
                        return 0;
                }
                glAttachShader(program, shader);
                glDeleteShader(shader);
        }

        glLinkProgram(program);
        if (had_error_program(program)) {
                char *msg = get_error_program(program);
                push_error_debug(msg);
                free(msg);
                glDeleteProgram(program);
                program = 0;
        }

        return program;
}
