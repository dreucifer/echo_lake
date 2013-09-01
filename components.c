#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "components.h"
#include "game.h"

#ifndef STEP_DX
#define STEP_DX 40
#endif

SDL_Rect * get_position(void * component) {
    position_p * self = component;
    return &(*self)->position;
}

component_p new_position(int x, int y, int w, int h,
        enum DIRECTION orientation) {
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
    self_pos->orientation = orientation;

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

    self_mot->delta_tx = 0;
    self_mot->delta_ty = 0;
    self_mot->delta_cx = 0;
    self_mot->delta_cx = 0;
    self_mot->vel_x = 0;
    self_mot->vel_y = 0;

    self->delegate = self_mot;
    self->destroy = destroy_motion;
    
    return self;
}

void add_motion(entity_p self, enum DIRECTION direction) {
    struct motion_s *self_mot = self->components[MOTION]->delegate;

    switch(direction) {
        default:
            self_mot->delta_cx += 0;
            self_mot->delta_tx += 0;
            self_mot->delta_cy += 0;
            self_mot->delta_ty += 0;
            self_mot->vel_x += 0;
            self_mot->vel_y += 0;
            break;
        case DOWN:
            self_mot->delta_ty = STEP_DX;
            self_mot->vel_y = 4;
            break;
        case UP:
            self_mot->delta_ty = -STEP_DX;
            self_mot->vel_y = -4;
            break;
        case RIGHT:
            self_mot->delta_tx = STEP_DX;
            self_mot->vel_x = 4;
            break;
        case LEFT:
            self_mot->delta_tx = -STEP_DX;
            self_mot->vel_x = -4;
            break;
    }
}

void clear_motion (entity_p self) {
    struct motion_s *self_mot = self->components[MOTION]->delegate;
    self_mot->delta_tx = 0;
    self_mot->delta_ty = 0;
    self_mot->delta_cx = 0;
    self_mot->delta_cx = 0;
    self_mot->vel_x = 0;
    self_mot->vel_y = 0;
}

void destroy_motion(component_p component) {
    free(component->delegate);
    free(component);
}

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

