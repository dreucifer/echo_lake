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
    enum DIRECTION orientation;
};

struct motion_s {
    int delta_tx, delta_cx;
    int delta_ty, delta_cy;
    int vel_x, vel_y;
};

struct texture_s {
    SDL_Texture *texture;
    char *filename;
    SDL_Rect section;
};

/* Position Component Functions */
component_p new_position(int x, int y, int w, int h,
        enum DIRECTION orientation);
SDL_Rect * get_position(void * component);
void destroy_position(component_p component);

/* Motion Component Functions */
component_p new_motion();
void add_motion(entity_p self, enum DIRECTION direction);
void clear_motion (entity_p self);
void destroy_motion(component_p component);

/* Motion Component Functions */
component_p new_texture(const char *texturepath, int w, int h);
int render_texture(void * component, SDL_Rect* dstrect);
void destroy_texture(component_p component);

#endif
