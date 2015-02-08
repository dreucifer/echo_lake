#include "menustate.h"
#include "stdio.h"

const char* menu_stateid = "MENU";

void menu_update() {
    break;
}

void menu_render() {
    break;
}

bool menu_onenter() {
    fprintf(stdout, "Entering State: %s", menu_stateid);
    return true;
}

bool menu_onexit() {
    fprintf(stdout, "Exiting State: %s", menu_stateid);
    return true;
}

struct state* menustate() {
    struct state* s_menustate = cmalloc(sizeof(struct state));
    s_menustate->update = &menu_update;
    s_menustate->render = &menu_render;
    s_menustate->onenter = &menu_onenter;
    s_menustate->onexit = &menu_onexit;
    s_menustate->stateid = menu_stateid;

    return s_menustate;
}
