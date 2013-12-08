#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#include "uthash.h"
#include "macros.h"

struct pool {
	struct entity *entities;
	bool init;
};
// get singleton of pool
struct pool 	*pool();
void 		pool_destroy(struct pool *self);
struct pool 	*pool_get();
int 		pool_start(struct pool *self);
int 		pool_stop(struct pool *self);
void 		pool_add_entity(
		struct pool **self, struct entity *entity);
struct entity 	*pool_get_entity(struct pool *self, const char *name);


struct entity {
	const char *name;
	struct component *components;
	UT_hash_handle hh;
};
struct entity 	*entity(const char *name);
void 		entity_add_component(
		struct entity **self, struct component *component);
void 		*entity_get_component(
		struct entity **self, const char *name);


struct component {
	const char *name;
	void *delegate;
	UT_hash_handle hh;
};
struct component *component(const char *name, void *delegate);


#endif
