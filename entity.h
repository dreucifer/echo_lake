#ifndef ENTITY_H
#define ENTITY_H

#define classproto(NAME) typedef struct NAME##_s NAME##_type, *NAME##_p

#include <stdbool.h>

classproto(world);
classproto(entity);
classproto(component);

enum COMPONENTS_LIST {
    POSITION,
    TEXTURE,
    MOTION,
    NUM_COMPONENTS
};

struct world_s {
    entity_p last, entities;
    _Bool init;
    world_p (*get)(); // get singleton of world
};

struct entity_s {
    entity_p next;
    component_p components[NUM_COMPONENTS];
    const char *name;
};

struct component_s {
    void * delegate;
    void (*destroy)(component_p self);
};

extern world_p get_world();
int init_world(world_p self);
void shutdown_world();
entity_p new_entity(const char *name);
int register_component(entity_p entity, enum COMPONENTS_LIST COMPONENT,
        component_p component);
void destroy_component(component_p self);

#endif
