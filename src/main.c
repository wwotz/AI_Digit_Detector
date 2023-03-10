#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "../include/window.h"
#include "../include/rect.h"
#include "../include/linear.h"
#include "../include/mnist_format.h"

#define WINDOW_NAME "MNIST Digit Detector"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_X SDL_WINDOWPOS_UNDEFINED
#define WINDOW_Y SDL_WINDOWPOS_UNDEFINED

#define NO_SHADER { GL_NONE, NULL }

#define WHITEBOARD_W (480)
#define WHITEBOARD_H (480)
#define WHITEBOARD_X (WINDOW_WIDTH-WHITEBOARD_W-50)
#define WHITEBOARD_Y ((WINDOW_HEIGHT-WHITEBOARD_H)/2)
#define WHITEBOARD_GLVS { GL_VERTEX_SHADER, "../shaders/whiteboard.glvs" }
#define WHITEBOARD_GLFS { GL_FRAGMENT_SHADER, "../shaders/whiteboard.glfs" }

#define AIBOARD_W (480)
#define AIBOARD_H (480)
#define AIBOARD_X (50)
#define AIBOARD_Y ((WINDOW_HEIGHT-AIBOARD_H)/2)
#define AIBOARD_GLVS { GL_VERTEX_SHADER, "../shaders/aiboard.glvs" }
#define AIBOARD_GLFS { GL_FRAGMENT_SHADER, "../shaders/aiboard.glfs" }

int running;

int main(int argc, char **argv)
{
        init_window(WINDOW_NAME, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

        if (had_error_debug()) {
                fprintf(stderr, "ERROR: %s\n", pop_error_debug());
                exit(EXIT_FAILURE);
        }

        identity_matrix(model);
        orthographic_matrix(projection, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, 10.0, -10.0);

        shader_info_t whiteboard_sinfo[3] = {
                WHITEBOARD_GLVS,
                WHITEBOARD_GLFS,
                NO_SHADER
        };


        rect_t whiteboard;
        init_rect(&whiteboard, WHITEBOARD_X, WHITEBOARD_Y, WHITEBOARD_W, WHITEBOARD_H, whiteboard_sinfo);

        if (had_error_debug()) {
                fprintf(stderr, "ERROR: %s\n", pop_error_debug());
                exit(EXIT_FAILURE);
        }

        shader_info_t aiboard_sinfo[3] = {
                AIBOARD_GLVS,
                AIBOARD_GLFS,
                NO_SHADER
        };

        rect_t aiboard;
        init_rect(&aiboard, AIBOARD_X, AIBOARD_Y, AIBOARD_W, AIBOARD_H, aiboard_sinfo);

        if (had_error_debug()) {
                fprintf(stderr, "ERROR: %s\n", pop_error_debug());
                exit(EXIT_FAILURE);
        }

        mnist_file_t mnist_file;
        mnist_file = open_file_mnist("../train-images-idx3-ubyte/train-images.idx3-ubyte");

        if (had_error_debug()) {
                fprintf(stderr, "ERROR: %s\n", pop_error_debug());
                exit(EXIT_FAILURE);
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        char *buffer = NULL;
        buffer = get_next_image_mnist(mnist_file, buffer, 0);

        unsigned int *pixels = malloc(28 * 28 * sizeof(*pixels));
        for (int i = 0; i < 28 * 28; i++) {
                char *bytes = (char *)(pixels+i);
                bytes[0] = buffer[i];
                bytes[1] = buffer[i];
                bytes[2] = buffer[i];
                bytes[3] = 0xff;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 28, 28, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        running = 1;
        SDL_Event event;
        while (running) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                running = 0;
                                break;
                        } else if (event.key.keysym.sym == SDLK_f) {
                                glBindTexture(GL_TEXTURE_2D, texture);
                                buffer = get_next_image_mnist(mnist_file, buffer, 0);
                                if (buffer == NULL || had_error_debug()) {
                                        printf("ERROR: %s\n", pop_error_debug());
                                        break;
                                }
                                unsigned int *pixels = malloc(28 * 28 * sizeof(*pixels));
                                for (int i = 0; i < 28 * 28; i++) {
                                        char *bytes = (char *)(pixels+i);
                                        bytes[0] = buffer[i];
                                        bytes[1] = buffer[i];
                                        bytes[2] = buffer[i];
                                        bytes[3] = 0xff;
                                }
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 28, 28, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                        }
                }
                clear_buffers_window(GL_COLOR);
                glBindTexture(GL_TEXTURE_2D, texture);
                render_rect(&whiteboard);
                glBindTexture(GL_TEXTURE_2D, 0);
                render_rect(&aiboard);
                swap_window();
        }

        free_rect(&whiteboard);
        free_rect(&aiboard);
        flush_debug();
        free_window();
        return 0;
}
