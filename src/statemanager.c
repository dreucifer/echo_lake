#include <stdlib.h>
#include "statemanager.h"

void push_state(struct state* new_state) {
    struct statenode *new_head = calloc(0, sizeof(struct statenode));
    new_head->data = new_state;
    new_head->next = statemanager()->head;
    statemanager()->head = new_head;
    statemanager()->head->data->onenter();
}

void pop_state() {
    struct statemanager *instance = statemanager();
    instance->head->data->onexit();
    instance->head = instance->head->next;
}

void change_state(struct state* new_state) {
    pop_state();
    push_state(new_state);
}

struct statemanager* statemanager() {
    static struct statemanager *s_statemanager = NULL;

    if(s_statemanager == NULL) {
        s_statemanager = calloc(0, sizeof(*s_statemanager));
        s_statemanager->head = NULL;
    }

    return s_statemanager;
}
