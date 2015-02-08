#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "state.h"

void menu_update();
void menu_render();

bool menu_onenter();
bool menu_onexit();

struct state* menustate();

#endif
