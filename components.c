#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "components.h"
#include "game.h"

#ifndef STEP_DX
#define STEP_DX 40
#endif

SDL_Rect * get_position(entity_p self) {
    position_p self_pos = self->components[POSITION]->delegate;
    return &(self_pos->position);
}

enum DIRECTION get_direction(entity_p self) {
    position_p self_pos = self->components[POSITION]->delegate;
    return self_pos->direction;
}

void set_position(entity_p self, SDL_Rect position) {
    position_p self_pos = self->components[POSITION]->delegate;
    self_pos->position = position;
}

void set_direction(entity_p self, enum DIRECTION direction) {
    position_p self_pos = self->components[POSITION]->delegate;
    self_pos->direction = direction;
}

component_p new_position(int x, int y, int w, int h,
        enum DIRECTION direction) {
    component_p self = malloc(sizeof(component_type));
    position_p self_pos = malloc(sizeof(position_type));
    if (self == NULL) {
        fprintf(stderr, "Could Not Create Component\n");
        return NULL;
    }

    if (self_pos == NULL) {
        fprintf(stderr, "Could Not Create Position\n");
        return NULL;
    }

    self_pos->position = (SDL_Rect){
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };
    self_pos->direction = direction;

    self->delegate = self_pos;
    self->destroy = destroy_position;

    return self;
}

void destroy_position(component_p component) {
    free(component->delegate);
    free(component);
}

component_p new_motion() {
    component_p self = malloc(sizeof(component_type));
    struct motion_s *self_mot = malloc(sizeof(struct motion_s));
    if (self == NULL) {
        fprintf(stderr, "Could Not Create Component\n");
        return NULL;
    }

    if (self_mot == NULL) {
        fprintf(stderr, "Could Not Create Motion\n");
        return NULL;
    }

    self_mot->delta_t = 0;
    self_mot->delta_c = 0;
    self_mot->vel = 0;

    self->delegate = self_mot;
    self->destroy = destroy_motion;
    
    return self;
}

void set_motion(entity_p self, int vel) {
    struct motion_s *self_mot = self->components[MOTION]->delegate;

    if (get_vel(self) == 0) {
        self_mot->delta_t = STEP_DX;
        self_mot->vel = vel;
    }
}

int get_vel(entity_p self) {
    struct motion_s *self_mot = self->components[MOTION]->delegate;
    return self_mot->vel;
}

void clear_motion (entity_p self) {
    struct motion_s *self_mot = self->components[MOTION]->delegate;
    self_mot->delta_t = 0;
    self_mot->delta_c = 0;
    self_mot->vel = 0;
}

void destroy_motion(component_p component) {
    free(component->delegate);
    free(component);
}

component_p new_texture(const char *image_path, int w, int h) {
    component_p self = malloc(sizeof(struct component_s));
    texture_p self_tex = malloc(sizeof(struct texture_s));
    SDL_Texture *new_tex;
    SDL_Surface *loaded_surface = IMG_Load(image_path);

    if (loaded_surface == NULL) {
        fprintf(stderr, "Unable to load image: %s! SDL_Image error: %s\n",
                image_path, IMG_GetError());
        return NULL;
    } else {
        new_tex = SDL_CreateTextureFromSurface(game_singleton()->renderer,
                loaded_surface);
    }

    self_tex->texture = new_tex;
    self_tex->section = (SDL_Rect){
        .x = 0,
        .y = 0,
        .w = w,
        .h = h
    };

    self->delegate = self_tex;
    self->destroy = destroy_texture;

    SDL_FreeSurface(loaded_surface);

    return self;
}

SDL_Texture* get_texture(entity_p self) {
    texture_p self_tex = self->components[TEXTURE]->delegate;
    return self_tex->texture;
}

SDL_Rect* get_section(entity_p self) {
    texture_p self_tex = self->components[TEXTURE]->delegate;
    return &(self_tex->section);
}

void destroy_texture(component_p component) {
    texture_p self = component->delegate;
    SDL_DestroyTexture(self->texture);
    free(self);
    free(component);
}
