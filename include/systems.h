#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "entity.h"

struct system {
    char *name;
    struct system *next;
};

int walk_entities(struct entity **cur_entity);
int render_entities(struct entity **cur_entity);
void camera_follow(struct entity **target);

#endif
