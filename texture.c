#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "texture.h"
#include "game.h"

#ifndef STEP_DX
#define STEP_DX 40
#endif

int render_texture(void * component, SDL_Rect* dstrect) {
    texture_p *self = component;
    SDL_RenderCopy(
            game_singleton()->renderer,
            (*self)->texture,
            &(*self)->section,
            dstrect);
    return 0;
}

void destroy_texture(component_p component) {
    texture_p self = component->delegate;
    SDL_DestroyTexture(self->texture);
    free(self);
    free(component);
}

component_p new_texture(const char *image_path, int w, int h) {
    SDL_Texture *new_texture;
    SDL_Surface *loaded_surface = IMG_Load(image_path);
    texture_p self_texture = malloc(sizeof(struct texture_s));
    component_p self = malloc(sizeof(struct component_s));

    if (loaded_surface == NULL) {
        fprintf(stderr, "Unable to load image: %s! SDL_Image error: %s\n",
                image_path, IMG_GetError());
        return NULL;
    } else {
        new_texture = SDL_CreateTextureFromSurface(game_singleton()->renderer,
                loaded_surface);
    }

    self_texture->texture = new_texture;
    self_texture->section = (SDL_Rect){
        .x = 0,
        .y = 0,
        .w = w,
        .h = h
    };

    self->delegate = self_texture;
    self->destroy = destroy_texture;

    SDL_FreeSurface(loaded_surface);

    return self;
}
