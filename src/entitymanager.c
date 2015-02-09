#include "entitymanager.h"

void add_entity(struct entity *entity)
{
    HASH_ADD_KEYPTR(hh, entitymanager()->entity_list,
                    entity->name,
                    strlen(entity->name),
                    entity);
}

struct entity *get_entity(const char *name)
{
    struct entity *self;
    HASH_FIND_STR(entitymanager()->entity_list, name, self);
    if (self) {
        return self;
    }

    return NULL;
}

struct entitymanager *entitymanager() {
    static struct entitymanager *self = NULL;

    if (self == NULL) {
        self = malloc(sizeof(struct entitymanager));
	self->add_entity = &add_entity;
	self->get_entity = &get_entity;
    }

    return self;
}