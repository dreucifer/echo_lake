/******** Components Header ********
 * Components are the data stores in a Entity-Component system.
 * They hold only a few data variables associated with some
 * discrete property of an entity. Also included are some simple
 * methods to set, get, and otherwise access and modify the data
 * in the components.
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SDL2/SDL.h>
#include "entity.h"

#ifndef POSITION_H
#define POSITION_H
/**** Position Component **** 
 * This here component holds various positions for the entity.
 * World, or map, position is where the entity is located in the world.
 * Render position is where the entity is on the screen.
 * Orientation is which direction the entity is currently facing:
 *   UP, DOWN, LEFT, RIGHT
 * */
classdef(position); // Macro in entity.h

// Position Enumerations and Structures
enum DIRECTION {
    UP, DOWN, LEFT, RIGHT
};

struct position_s {
    SDL_Rect world_pos;
    enum DIRECTION dir;
};

// Position Functions
component_p     new_pos(SDL_Rect world_pos, enum DIRECTION dir);
void            del_pos(component_p self);
int             init_pos(entity_p self);
SDL_Rect*       get_world_pos(entity_p self);
SDL_Rect        get_screen_pos(entity_p self);
enum DIRECTION  get_dir(entity_p self);
void            set_world_pos(entity_p self, SDL_Rect pos);
void            set_dir(entity_p self, enum DIRECTION dir);
#endif

#ifndef MOTION_H
#define MOTION_H
/**** Motion Component **** 
 * This is the component that holds motion information. We really just
 * need a couple of deltas and the velocity. The system the applies 
 * the motion to another component will determine the direction of the
 * motion, only the total displacement, current displacement, and 
 * velocity are stored.
 * */
classdef(motion);

// Motion Enums and Structures
struct motion_s {
    int delta_t, delta_c, vel;
};

// Motion Functions
component_p new_motion();
void        set_motion(entity_p self, int vel);
int         get_vel(entity_p self);
int         get_delta_t(entity_p self);
int         get_delta_c(entity_p self);
void        clear_motion (entity_p self);
void        del_motion(component_p component);
#endif

#ifndef TEXTURE_H
#define TEXTURE_H
/**** Texture Component **** 
 * */
classdef(texture);

// Texture Enums and Structs
struct texture_s {
    SDL_Texture *texture;
    char *filename;
    SDL_Rect section;
};

/* Texture Component Functions */
component_p new_texture(const char *texturepath, int w, int h);
SDL_Texture* get_texture(entity_p self);
SDL_Rect* get_section(entity_p self);
void set_section(entity_p self, SDL_Rect *section);
void del_texture(component_p component);
#endif

#endif
