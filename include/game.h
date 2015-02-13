#ifndef GAME_H
#define GAME_H

#define WIDTH 640
#define HEIGHT 480
#define BPP 0
#define DEPTH 32
#define FPS 30
#define PLAYER_SIZE 32
#define FRAMESKIP 5
#define TITLE "echo lake"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entitymanager.h"
#include "statemanager.h"
#include "world.h"

extern const int SKIP_TICKS;

struct game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct entitymanager *entitymanager;
    struct statemanager *statemanager;
    struct world *world;

    char *title;
    bool init;

    int (*preload)();
    int (*create)();
    int (*update)();
    int (*render)();
    void (*cleanup)();
};

struct game *game();
int game_init(struct game *self);
void game_destroy();

#endif
