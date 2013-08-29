#ifndef ACTOR_H
#define ACTOR_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum orientation_e {
    CHAR_UP, CHAR_DOWN, CHAR_LEFT, CHAR_RIGHT
} ORIENTATION;

typedef struct location_s {
    int x, y;
} location_type, *location_p;

typedef struct actor_s {
    SDL_Texture *image;
    SDL_Rect clipping;
    location_type;
    ORIENTATION orient;
    void (*set_sprite)(void * self);
} actor_type, *actor_p;

#endif
