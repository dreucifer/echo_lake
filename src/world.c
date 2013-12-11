#include "game.h"
#include "world.h"
#include "systems.h"

struct world *world()
{
	struct world *self = malloc(sizeof(struct world));
	world_start(self);

	return self;
}

void world_destroy(struct world *self)
{
	free(self->entities);
	free(self);
	self = NULL;
}

struct world *world_get()
{
	static struct world *self;

	if (self == NULL) {
		self = world();
	}

	if (self != NULL && self->init == false) {
		if(world_start(self) != 0) {
			fprintf(stderr, "\nCould Not Initialize World");
			return NULL;
		}
	} else if (self != NULL && self->init == true) {
		return self;
	} else {
		return NULL;
	}

	return NULL;
}

int world_start(struct world *self)
{
	*self = (struct world){

		.bounds = { WIDTH, HEIGHT },
		.cam = { 0, 0 },
		.entities = NULL,
		.init = true,
	};

	return 0;
}

int world_stop(struct world *self)
{
	return self->init;
}

void world_add_entity(struct entity *entity)
{
	HASH_ADD_KEYPTR(hh, world_get()->entities,
			entity->name,
			strlen(entity->name),
			entity);
}

struct entity *world_get_entity(const char *name)
{
	struct entity *self;
	HASH_FIND_STR(world_get()->entities, name, self);
	if (self) {
		return self;
	}

	return NULL;
}

int world_render() {
	return render_entities(&world_get()->entities);
}
