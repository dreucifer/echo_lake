#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "entity.h"

struct system {
	char *name;
	struct system *next;
};

int walk_entities(struct entity *entity);
int render_entities(struct entity *entity);

#endif
