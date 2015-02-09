#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

struct state {
    void (*update)();
    void (*render)();
    bool (*onenter)();
    bool (*onexit)();

    const char* stateid;
};

#endif
