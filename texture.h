#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include "entity.h"

typedef struct texture_s {
    SDL_Texture *texture;
    char *filename;
    SDL_Rect section;
} texture_type, *texture_p;

int render_texture(void * component, SDL_Rect* dstrect);
component_p new_texture(const char *texturepath, int w, int h);
void destroy_texture(component_p component);

#endif
