#include <stdio.h>
#include <stdlib.h>
#include "menustate.h"
#include "entitymanager.h"
#include "systems.h"

const char* menu_stateid = "MENU";

void menu_update() {
}

void menu_render() {
    render_entities(&entitymanager()->entity_list);
}

bool menu_onenter() {
    fprintf(stderr, "[INFO] MenuState::onenter()\n");
    return true;
}

bool menu_onexit() {
    fprintf(stdout, "[INFO] MenuState::onexit()");
    return true;
}

struct state* menustate() {
    struct state* s_menustate = malloc(sizeof(struct state));
    s_menustate->update = &menu_update;
    s_menustate->render = &menu_render;
    s_menustate->onenter = &menu_onenter;
    s_menustate->onexit = &menu_onexit;
    s_menustate->stateid = menu_stateid;

    return s_menustate;
}
