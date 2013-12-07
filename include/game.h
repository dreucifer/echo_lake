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

struct game {
	SDL_Window *window;
	SDL_Renderer *renderer;
	char *title;
	bool init;

	struct state;
};

struct state {
	int (*preload)(struct game *self);
	int (*create)(struct game *self);
	int (*update)(struct game *self);
	int (*render)(struct game *self);
	int (*destroy)(struct game *self);
};

struct game *game(const char *title);
struct game *game_get();
int game_init(struct game *self);
void game_destroy(struct game *self);

#endif
