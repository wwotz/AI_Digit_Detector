#ifndef WINDOW_H_
#define WINDOW_H_

#include "deps.h"

#define DEFAULT_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL \
                       | SDL_WINDOW_RESIZABLE)

typedef enum WINDOW_ERROR {
        WINDOW_NO_ERROR = 0, WINDOW_SDL_ERROR,
        WINDOW_TTF_ERROR, WINDOW_IMG_ERROR,
        WINDOW_GLEW_ERROR, WINDOW_WINDOW_ERROR,
        WINDOW_CONTEXT_ERROR, WINDOW_ENUM_COUNT
} WINDOW_ERROR;

int init_window(const char *name, int x, int y, int w, int h, int flags);
const char *get_error_window(int status);
void clear_buffers_window(int buffers);
void swap_window(void);
void get_size_window(int *w, int *h);
void resize_window(int w, int h);
void events_window(SDL_Event *event);
void free_window(void);

#endif // WINDOW_H_
