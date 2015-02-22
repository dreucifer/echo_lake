#include "entitymanager.h"

static void add_entity(struct entity *entity) {
  struct entity **head = &entitymanager()->entity_list;
    fprintf(stderr, "[INFO] adding entity %s\n", entity->name);
    HASH_ADD_STR(*head, name, entity);
}

static struct entity *get_entity(const char *name)
{
    struct entity *self;
    HASH_FIND_STR(entitymanager()->entity_list, name, self);
    if (self) {
        return self;
    }

    return NULL;
}

static void cleanup() {
    struct entitymanager *instance = entitymanager();
    struct entity *current_entity, *tmp;

    HASH_ITER(hh, instance->entity_list, current_entity, tmp) {
        HASH_DEL(instance->entity_list, current_entity);
        current_entity->cleanup(current_entity);
        free(current_entity);
    }
}

struct entitymanager *entitymanager() {
    static struct entitymanager *self = NULL;

    if (self == NULL) {
        self = malloc(sizeof(struct entitymanager));
        self->entity_list = NULL;
        self->add_entity = &add_entity;
        self->get_entity = &get_entity;
        self->cleanup = &cleanup;
    }

    return self;
}
