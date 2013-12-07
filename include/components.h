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

/**** Position Component **** 
 * This here component holds various positions for the entity.
 * World, or map, position is where the entity is located in the world.
 * Render position is where the entity is on the screen.
 * Orientation is which direction the entity is currently facing:
 *   UP, DOWN, LEFT, RIGHT
 * */

// Position Enumerations and Structures
typedef enum DIRECTION {
	UP, DOWN, LEFT, RIGHT
} Direction;

struct position {
	SDL_Rect world_pos;
	enum DIRECTION dir;
};

// Position Functions
struct component	*position(SDL_Rect world_pos, Direction dir);
int			position_init(struct entity *self);
SDL_Rect 		position_get_world(struct entity *self);
SDL_Rect		position_get_screen(struct entity *self);
enum DIRECTION		position_get_dir(struct entity *self);
void			position_set_world(struct entity *self, SDL_Rect pos);
void			position_set_screen(struct entity *self, SDL_Rect pos);
void			position_set_dir(struct entity *self, enum DIRECTION dir);
void			position_destrou(struct component *self);

/**** Motion Component **** 
 * This is the component that holds motion information. We really just
 * need a couple of deltas and the velocity. The system the applies 
 * the motion to another component will determine the direction of the
 * motion, only the total displacement, current displacement, and 
 * velocity are stored.
 * */
// Motion Enums and Structures
struct motion {
	int delta_t, delta_c, vel;
};

// Motion Functions
struct component	*motion();
void			motion_set(struct entity *self, int vel);
int			motion_get_vel(struct entity *self);
int			motion_get_delta_t(struct entity *self);
int			motion_get_delta_c(struct entity *self);
void			motion_clear(struct entity *self);
void			motion_destroy(struct component *component);

/**** Texture Component **** 
 * */
// Texture Enums and Structs
struct texture {
	SDL_Texture *image;
	char *filename;
	SDL_Rect section;
};

/* Texture Component Functions */
struct component 	*texture(const char *texturepath, int w, int h);
struct component 	*from_texture(SDL_Texture *new_tex, int w, int h);
SDL_Texture*		texture_get_image(struct entity *self);
SDL_Rect* 		texture_get_section(struct entity *self);
void 			texture_set_section(
				struct entity *self, SDL_Rect *section);
void 			texture_destroy(struct component *component);

#endif
