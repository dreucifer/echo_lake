#ifndef ENTITY_H
#define ENTITY_H

#include "macros.h"
#include <stdbool.h>

struct world {
	struct entity *entities;
	struct component *components;
	bool init;
};
// get singleton of world
struct world 	*world();
struct world 	*world_get();
int 		world_start(struct world *self);
int 		world_stop(struct world *self);
void 		world_destroy(struct world *self);
void 		world_add_entity(
		struct world **self, struct entity *entity);


struct entity {
	const char *name;
	struct component *components;
	struct entity *next;
};
struct entity 	*entity(const char *name);
struct entity 	*entity_get_component(
		struct entity **self, const char *name);
void 		entity_add_component(
		struct entity **self, struct component *component);


struct component {
	struct entity *entities;
	void *delegate;
};
void component(struct component *self);


#endif
