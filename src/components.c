#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "components.h"
#include "game.h"

#ifndef STEP_DX
#define STEP_DX 40
#endif

// Position Component Functions
struct component 	*position(
    SDL_Point pos,
    SDL_Point size,
    SDL_Point offset,
    Direction dir)
{
    struct position *self = (struct position *)malloc(sizeof(struct position));
    if (self == NULL) {
        fprintf(stderr, "Could Not Create Position\n");
        return NULL;
    }

    *self= (struct position) {
        .pos = pos,
         .size = size,
          .offset = offset,
           .dir = dir
    };

    return component("position", self);
}
struct position *position_from_entity(struct entity **self)
{
    return (struct position *)entity_get_component(self, "position");
}

SDL_Point *position_get_world(struct entity **self)
{
    return &position_from_entity(self)->pos;
}

Direction position_get_dir(struct entity **self)
{
    return position_from_entity(self)->dir;
}

void position_set_world(struct entity **self, SDL_Point pos)
{
    position_from_entity(self)->pos = pos;
}

void position_set_dir(struct entity **self, Direction dir)
{
    position_from_entity(self)->dir = dir;
}

void position_destroy(struct component **self)
{
    free((struct position *)(*self)->delegate);
    free(*self);
    *self = NULL;
}
// End Position Functions

// Motion Component Functions
struct component *motion()
{
    struct motion *self = malloc(sizeof(struct motion));
    if (self == NULL) {
        fprintf(stderr, "Could Not Create Motion\n");
        return NULL;
    }

    *self = (struct motion) {
        .delta_t = 0,
         .delta_c = 0,
          .vel = 0
    };

    return component("motion", self);
}

struct motion *motion_from_entity(struct entity **self)
{
    return (struct motion *)entity_get_component(self, "motion");
}

void motion_clear(struct entity **self)
{
    *motion_from_entity(self) = (struct motion) {
        .delta_t = 0,
         .delta_c = 0,
          .vel = 0
    };
}

void motion_set(struct entity **self, int vel)
{
    struct motion *self_mot = motion_from_entity(self);

    if (motion_get_vel(self) == 0) {
        self_mot->delta_t = STEP_DX;
        self_mot->vel = vel;
    }
}

int motion_get_vel(struct entity **self)
{
    return motion_from_entity(self)->vel;
}

int motion_get_delta_t(struct entity **self)
{
    return motion_from_entity(self)->delta_t;
}

int motion_get_delta_c(struct entity **self)
{
    return motion_from_entity(self)->delta_c;
}

void motion_destroy(struct component **component)
{
    free((*component)->delegate);
    free(*component);
    *component = NULL;
}
// End of Motion Component


// Texture Component
struct component *texture(const char *image_path, int w, int h)
{
    struct texture *self = malloc(sizeof(struct texture));
    SDL_Texture *image;
    SDL_Surface *temp = IMG_Load(image_path);

    if (temp == NULL) {
        fprintf(stderr, "Unable to load image: %s! SDL_Image error: %s\n",
                image_path, IMG_GetError());
        return NULL;
    } else {
        image = SDL_CreateTextureFromSurface(
                    game()->renderer,
                    temp);
    }

    *self = (struct texture) {
        .image = image,
        .section = (SDL_Rect) {
            .x = 0,
             .y = 0,
              .w = w,
               .h = h
        }
    };

    SDL_FreeSurface(temp);

    return component("texture", self);
}

struct component *texture_from_texture(
    SDL_Texture *image, int w, int h)
{
    struct texture *self = malloc(sizeof(struct texture));

    *self = (struct texture) {
        .image = image,
        .section = (SDL_Rect) {
            .x = 0,
             .y = 0,
              .w = w,
               .h = h
        }
    };

    return component("texture", self);
}

struct texture *texture_from_entity(struct entity **self)
{
    return (struct texture *)entity_get_component(self, "texture");
}

SDL_Texture* texture_get_image(struct entity **self)
{
    return texture_from_entity(self)->image;
}

SDL_Rect texture_get_section(struct entity **self)
{
    return texture_from_entity(self)->section;
}

void texture_set_section(struct entity **self, SDL_Rect *section)
{
    texture_from_entity(self)->section = *section;
}

void texture_destroy(struct component **component)
{
    struct texture *self = (*component)->delegate;
    SDL_DestroyTexture(self->image);
    free(self);
    free(component);
    component = NULL;
}
