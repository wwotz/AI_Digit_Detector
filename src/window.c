#include "../include/window.h"

static SDL_Window *window;
static SDL_GLContext *context;
extern int running;

/* initialise SDL, TTF, IMG and OpenGL context */
int init_window(const char *name, int x, int y, int w, int h, int flags)
{
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                push_error_debug(get_error_window(WINDOW_SDL_ERROR));
                return WINDOW_SDL_ERROR;
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
                push_error_debug(get_error_window(WINDOW_IMG_ERROR));
                return WINDOW_IMG_ERROR;
        }

        if (TTF_Init() != 0) {
                push_error_debug(get_error_window(WINDOW_TTF_ERROR));
                return WINDOW_TTF_ERROR;
        }

        if ((window = SDL_CreateWindow(name, x, y, w, h, flags | DEFAULT_FLAGS)) == NULL) {
                push_error_debug(get_error_window(WINDOW_WINDOW_ERROR));
                return WINDOW_WINDOW_ERROR;
        }

        if ((context = SDL_GL_CreateContext(window)) == NULL) {
                push_error_debug(get_error_window(WINDOW_CONTEXT_ERROR));
                return WINDOW_CONTEXT_ERROR;
        }

        int status;
        if ((status = glewInit()) != GLEW_OK) {
                push_error_debug(get_error_window(WINDOW_GLEW_ERROR));
                return WINDOW_GLEW_ERROR;
        }

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glViewport(0.0, 0.0, w, h);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return WINDOW_NO_ERROR;
}

/* return respective error messages based on status on init_window */
const char *get_error_window(int status)
{
        static const char *msgs[WINDOW_ENUM_COUNT] = {
                "No Error",
                "Failed to initialise SDL!",
                "Failed to initialise TTF!",
                "Failed to initialise IMG!",
                "Failed to initialise GLEW/OpenGL!",
                "Failed to create window!",
                "Failed to create opengl context!",
        };

        return (status >= 0 && status < WINDOW_ENUM_COUNT
                ? msgs[status] : "Unknown Error");
}

/* clear buffers for the OpenGL window */
void clear_buffers_window(int buffers)
{
        static const GLfloat colour[4] = {
                0.1, 0.1, 0.1, 1.0
        };

        glClearBufferfv(buffers, 0, colour);
}

/* present the contents of the current frame */
void swap_window()
{
        SDL_GL_SwapWindow(window);
}

/* place the window dimensions in variables @w and @h respectively */
void get_size_window(int *w, int *h)
{
        SDL_GetWindowSize(window, w, h);
}

/* resize the window and the OpenGL viewport,
  as well as updating the offsets for the mouse */
void resize_window(int w, int h)
{
        glViewport(0.0, 0.0, w, h);
}

/* take action on the current window events */
void events_window(SDL_Event *event)
{
        if (event->type == SDL_WINDOWEVENT) {
                switch (event->window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                        int w, h;
                        get_size_window(&w, &h);
                        resize_window(w, h);
                        break;
                case SDL_WINDOWEVENT_CLOSE:
                        running = 0;
                        break;
                }
        }
}

/* free the window and OpenGL context */
void free_window(void)
{
        SDL_Quit();
        IMG_Quit();
        TTF_Quit();

        SDL_DestroyWindow(window);
        SDL_GL_DeleteContext(context);
        window = NULL;
        context = NULL;
}
