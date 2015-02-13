/* Entity-Component System*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entity.h"

static void cleanup(struct entity *self) {
    if (self->data != NULL) {
        free(self->data);
    }
}

void entity_add_component(
    struct entity **self, struct component *component)
{
    HASH_ADD_KEYPTR(hh, (*self)->components,
                    component->name,
                    strlen(component->name),
                    component);
}

void *entity_get_component(struct entity **self, const char *name)
{
    struct component *self_comp = NULL;
    HASH_FIND_STR((*self)->components, name, self_comp);
    if (self_comp) {
        return self_comp->delegate;
    }

    return NULL;
}

struct component *component(const char *name, void *delegate)
{
    struct component *self = malloc(sizeof(struct component));
    *self = (struct component) {
        .name = strdup(name),
         .delegate = delegate
    };

    return self;
}

struct entity *entity(const char *name, void **data)
{
    struct entity *self = malloc(sizeof(struct entity));
    *self = (struct entity) {
        .name = strdup(name),
         .data = data,
          .components = NULL,
           .cleanup = &cleanup
    };

    return self;
}
