#ifndef WORLD
#define WORLD
#include <stdbool.h>
#include "entity.h"

struct world {
	SDL_Point bounds, cam;
	struct entity *entities;
	struct tilemap *map;
	bool init;
};

struct world 	*world();
void 		world_destroy(struct world *self);
struct world 	*world_get();
int 		world_start(struct world *self);
int 		world_stop(struct world *self);
void 		world_add_entity(struct entity *entity);
struct entity 	*world_get_entity(const char *name);
int 		world_render();



#endif
