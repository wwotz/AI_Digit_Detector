#include "../include/textures.h"
#include <arpa/inet.h>

/* basic texture function, needs more to be added
   in the future, needs to be further tested,
   can potentially cause segfault on some textures
   that have not been tested */

GLuint create_texture(texture_info_t info)
{
        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (info.type == TEXTURE_PIXELS) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.w, info.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.pixels);
        } else if (info.type == TEXTURE_FILE) {
                SDL_Surface *surface;
                GLuint *pixels;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_CLAMP_TO_EDGE);

                surface = IMG_Load(info.file);
                if (!surface) {
                        push_error_debug("File does not exist");
                        glDeleteTextures(1, &texture);
                        return 0;
                }

                switch (surface->format->BytesPerPixel) {
                case 1: {
                        char *index;
                        SDL_Color colour;
                        GLuint pixel = 0;

                        pixels = malloc(sizeof(*pixels) * surface->w * surface->h);
                        for (int i = 0; i < surface->w * surface->h; i++) {
                                index = (char *) surface->pixels+i;
                                colour = surface->format->palette->colors[*index];
                                pixel |= (pixel << 8) | colour.r;
                                pixel |= (pixel << 8) | colour.g;
                                pixel |= (pixel << 8) | colour.b;
                                pixel |= (pixel << 8) | colour.a;
                                pixels[i] = pixel;
                        }

                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                        free(pixels);
                        break;
                }

                case 2: {
                        push_error_debug("2 byte textures not yet supported!");
                        return 0;
                }

                case 3: {
                        push_error_debug("3 byte textures not yet supported!");
                        return 0;
                }

                case 4: {
                        pixels = surface->pixels;
                        break;
                }
                }

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                SDL_FreeSurface(surface);
        } else {
                push_error_debug("Invalid Texture Type!");
                glDeleteTextures(1, &texture);
                texture = 0;
        }

        return texture;
}

/* a wrapper around the OpenGL call to delete textures */
void delete_texture(GLuint texture)
{
        glDeleteTextures(1, &texture);
}
