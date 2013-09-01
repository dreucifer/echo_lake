#ifndef GAME_H
#define GAME_H

#define WIDTH 512
#define HEIGHT 448
#define BPP 0
#define DEPTH 32
#define FPS 30
#define PLAYER_SIZE 32
#define FRAMESKIP 5
#define TITLE "echo lake"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"

extern const int SKIP_TICKS;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, NUM_KEYS
};

typedef struct game_s game_type, *game_p;

struct game_s {
    SDL_Window *window;
    SDL_Renderer *renderer;
    char *title;
    bool init;

    int (*handle)(game_p self);
    int (*update)(game_p self);
    int (*render)(game_p self);
};

game_p game_constructor(const char *title);
int game_initialize(game_p self);
void game_destructor();
game_p game_singleton();
int game_input();
int game_logic();
int game_render();
bool handle_input(entity_p entity);
bool should_exit(SDL_Event *event_pointer);

#endif
