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

        mnist_header_t header;
        header = read_header_mnist("../train-images-idx3-ubyte/train-images.idx3-ubyte");

        if (had_error_debug()) {
                fprintf(stderr, "ERROR: %s\n", pop_error_debug());
                exit(EXIT_FAILURE);
        }

        printf("magic: %d\n", header.magic);
        printf("image count: %d\n", header.image_count);


        running = 1;
        SDL_Event event;
        while (running) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                                running = 0;
                                break;
                        }
                }
                clear_buffers_window(GL_COLOR);
                render_rect(&whiteboard);
                render_rect(&aiboard);
                swap_window();
        }

        free_rect(&whiteboard);
        free_rect(&aiboard);
        flush_debug();
        free_window();
        return 0;
}
