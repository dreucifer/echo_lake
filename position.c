#include <stdlib.h>
#include <stdio.h>
#include "position.h"

SDL_Rect * get_position(void * component) {
    position_p * self = component;
    return &(*self)->position;
}

component_p new_position(int x, int y, int w, int h,
        ORIENT orientation) {
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
