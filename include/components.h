/**** Components Header ****
 * Components are the data stores in a Entity-Component system.
 * They hold only a few data variables associated with some
 * discrete property of an entity. Also included are some simple
 * methods to set, get, and otherwise access and modify the data
 * in the components.
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SDL2/SDL.h>
#include <uthash.h>
#include "entity.h"

/**** Component Definition ****
 * hh: Components are stored in a hash using uthash library.
 * name: Unique description of the component, used as the hash key.
 * delegate: This holds the address of the actual component structures
 * 	below.
 *
 * */
struct component {
    UT_hash_handle hh;
    const char *name;
    void *delegate;
};
struct component 	*component(const char *name, void *delegate);



/**** Position Component ****
 * This here component holds orientation data for the entity.
 * position is defined as an SDL_Point containing the x, y
 *	coordinates of the entity.
 * size is the width and height of the entity.
 * offset is the percentage offset of the position point,
 * 	i.e 50, 50 would be the center.
 * Orientation is the direction the entity is currently facing:
 * 	UP, DOWN, LEFT, RIGHT
 * */

typedef enum DIRECTION {
    UP, DOWN, LEFT, RIGHT
} Direction;

struct position {
    SDL_Point pos;
    SDL_Point size;
    SDL_Point offset;
    Direction dir;
};

struct component 	*position(
    SDL_Point pos,
    SDL_Point size,
    SDL_Point offset,
    Direction dir);
struct position 	*position_from_entity(struct entity **self);
SDL_Point 		*position_get_world(struct entity **self);
Direction		position_get_dir(struct entity **self);
void			position_set_world(struct entity **self, SDL_Point pos);
void			position_set_dir(struct entity **self, Direction dir);
void			position_destroy(struct component **self);

/**** Motion Component ****
 * This is the component that holds motion information. We really just
 * need a couple of deltas and the velocity. The system the applies
 * the motion to another component will determine the direction of the
 * motion, only the total displacement, current displacement, and
 * velocity are stored.
 * */
struct motion {
    int delta_t, delta_c, vel;
};

struct component	*motion();
struct motion 		*motion_from_entity(struct entity **self);
void			motion_clear(struct entity **self);
void			motion_set(struct entity **self, int vel);
int			motion_get_vel(struct entity **self);
int			motion_get_delta_t(struct entity **self);
int			motion_get_delta_c(struct entity **self);
void			motion_destroy(struct component **component);

/**** Texture Component ****
 * */
struct texture {
    SDL_Texture *image;
    char *filename;
    SDL_Rect section;
};

struct component 	*texture(const char *texturepath, int w, int h);
struct component 	*texture_from_texture(
    SDL_Texture *new_tex, int w, int h);
struct texture 		*texture_from_entity(struct entity **self);
SDL_Texture*		texture_get_image(struct entity **self);
SDL_Rect  		texture_get_section(struct entity **self);
void 			texture_set_section(
    struct entity **self, SDL_Rect *section);
void 			texture_destroy(struct component **component);

#endif
