#ifndef STATE_H
#define STATE_H

struct state {
    void (*update)();
    void (*render)();
    bool (*onenter)();
    bool (*onexit)();

    const char* stateid;
};

#endif
