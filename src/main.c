#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "../include/window.h"

#define WINDOW_NAME "MNIST Digit Detector"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_X SDL_WINDOWPOS_UNDEFINED
#define WINDOW_Y SDL_WINDOWPOS_UNDEFINED

int running;

int main(int argc, char **argv)
{
        init_window(WINDOW_NAME, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

        if (had_error_debug()) {
                fprintf(stderr, "ERROR: %s\n", pop_error_debug());
                exit(EXIT_FAILURE);
        }

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
                swap_window();
        }

        flush_debug();
        free_window();
        return 0;
}
