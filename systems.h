#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "entity.h"

struct system_s {
    char *name;
    struct system_s *next;
};

int walk_entities(entity_p entity);
int render_entities(entity_p entity);

#endif
