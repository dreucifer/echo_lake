#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"

struct entitymanager {
  struct entity *entity_list;
  void (*add_entity)(struct entity *entity);
  struct entity* (*get_entity)(const char *name);
};

void add_entity(struct entity *entity);
struct entity *get_entity(const char *name);
struct entitymanager *entitymanager();

#endif