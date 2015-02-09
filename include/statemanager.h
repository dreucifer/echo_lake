#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "state.h"

struct statenode {
    struct state *data;
    struct statenode *next;
};

struct statemanager {
    struct statenode *head;
    void (*push_state)(struct state *new_state);
    void (*pop_state)();
    void (*change_state)(struct state *new_state);
};

void push_state(struct state *new_state);
void pop_state();
void change_state(struct state *new_state);
struct statemanager *statemanager();

#endif
