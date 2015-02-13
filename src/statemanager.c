#include <stdlib.h>
#include "statemanager.h"

void push_state(struct state* new_state) {
    struct statenode *new_head = malloc(sizeof(struct statenode));
    new_head->data = new_state;
    new_head->next = statemanager()->head;
    statemanager()->head = new_head;
    statemanager()->head->data->onenter();
}

void pop_state() {
    struct statemanager *instance = statemanager();
    if (instance->head != NULL ) {
        instance->head->data->onexit();
        instance->head = instance->head->next;
    }
}

void change_state(struct state* new_state) {
    pop_state();
    push_state(new_state);
}

struct statemanager *statemanager() {
    static struct statemanager *self = NULL;

    if (self == NULL) {
        self = malloc(sizeof(struct statemanager));
        self->head = NULL;
        self->change_state = &change_state;
        self->push_state = &push_state;
        self->pop_state = &pop_state;
    }

    return self;
}
