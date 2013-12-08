/* Entity-Component System*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entity.h"

struct pool *pool()
{
	struct pool *self = malloc(sizeof(struct pool));
	pool_start(self);

	return self;
}

void pool_destroy(struct pool *self)
{
	free(self->entities);
	free(self);
	self = NULL;
}

struct pool *pool_get()
{
	static struct pool *self;

	if (self == NULL) {
		self = pool();
	}

	if (self != NULL && self->init == false) {
		if(pool_start(self) != 0) {
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

int pool_start(struct pool *self)
{
	*self = (struct pool){
		.entities = NULL,
		.init = true,
	};

	return 0;
}

int pool_stop(struct pool *self)
{
	return self->init;
}

void pool_add_entity(struct pool **head, struct entity *entity)
{
	HASH_ADD_KEYPTR(hh, (*head)->entities,
			entity->name,
			strlen(entity->name),
			entity);
}

struct entity *pool_get_entity(struct pool *head, const char *name)
{
	struct entity *self;
	HASH_FIND_STR(head->entities, name, self);
	if (self) {
		return self;
	}

	return NULL;
}

struct entity *entity(const char *name)
{
	struct entity *self = malloc(sizeof(struct entity));
	*self = (struct entity){
		.name = strdup(name),
		.components = NULL
	};

	return self;
}

void entity_add_component(
		struct entity **self, struct component *component)
{
	HASH_ADD_KEYPTR(hh, (*self)->components,
			component->name,
			strlen(component->name),
			component);
}

void *entity_get_component(struct entity **head, const char *name)
{
	struct component *self;
	HASH_FIND_STR((*head)->components, name, self);
	if (self) {
		return self->delegate;
	}

	return NULL;
}

struct component *component(const char *name, void *delegate)
{
	struct component *self = malloc(sizeof(struct component));
	*self = (struct component){
		.name = strdup(name),
		.delegate = delegate
	};

	return self;
}
