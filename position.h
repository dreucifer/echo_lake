#ifndef POSITION_H
#define POSITION_H

#include <SDL2/SDL.h>
#include "entity.h"

typedef enum orient_e {
    CH_UP, CH_DOWN, CH_LEFT, CH_RIGHT
} ORIENT;

typedef struct position_s {
    SDL_Rect position;
    ORIENT orientation;
} position_type, *position_p;

SDL_Rect * get_position(void * component);
component_p new_position(int x, int y, int w, int h,
        ORIENT orientation);
void destroy_position(component_p component);

#endif
