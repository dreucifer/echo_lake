/* Entity-Component System*/
#include <stdlib.h>
#include <stdio.h>
#include "entity.h"

world_p get_world() {
    static world_p self;

    if (self == NULL) {
        self = malloc(sizeof(struct world_s));
        init_world(self);
    }
    
    if (self != NULL && self->init == false) {
        if(init_world(self) != 0) {
            fprintf(stderr, "\nCould Not Initialize world_p");
            return NULL;
        }
    } else if (self != NULL && self->init == true) {
        return self;
    } else {
        return NULL;
    }
    return NULL;
}

int init_world(world_p self) {
    self->entities = NULL;
    self->last = NULL;
    self->init = true;
    self->get = &get_world;

    return 0;
}

entity_p new_entity() {
    world_p self = get_world();
    entity_p new_entity = malloc(sizeof(struct entity_s));

    new_entity->next = NULL;
    for (int i = 0; i < NUM_COMPONENTS; i++) {
        new_entity->components[i] = NULL;
    }

    if (self != NULL && new_entity != NULL) {
        if (self->entities == NULL) {
            self->entities = new_entity;
            self->last = new_entity;
        } else {
            self->last->next = new_entity;
        }
    }

    return new_entity;
}

int register_component(entity_p entity, enum COMPONENTS_LIST COMPONENT,
        component_p component) {
    entity->components[COMPONENT] = component;

    return 0;
}
