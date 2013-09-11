#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SDL2/SDL.h>
#include "entity.h"

typedef struct position_s position_type, *position_p;
typedef struct motion_s motion_type, *motion_p;
typedef struct texture_s texture_type, *texture_p;

enum DIRECTION {
    UP, DOWN, LEFT, RIGHT
};

struct position_s {
    SDL_Rect position;
    enum DIRECTION direction;
};

struct motion_s {
    int delta_t, delta_c, vel;
};

struct texture_s {
    SDL_Texture *texture;
    char *filename;
    SDL_Rect section;
};

/* Position Component Functions */
component_p new_position(int x, int y, int w, int h,
        enum DIRECTION direction);
SDL_Rect * get_position(entity_p self);
enum DIRECTION get_direction(entity_p self);
void set_position(entity_p self, SDL_Rect position);
void set_direction(entity_p self, enum DIRECTION direction);
void destroy_position(component_p component);

/* Motion Component Functions */
component_p new_motion();
void set_motion(entity_p self, int vel);
int get_vel(entity_p self);
void clear_motion (entity_p self);
void destroy_motion(component_p component);

/* Texture Component Functions */
component_p new_texture(const char *texturepath, int w, int h);
SDL_Texture* get_texture(entity_p self);
SDL_Rect* get_section(entity_p self);
void destroy_texture(component_p component);

#endif
