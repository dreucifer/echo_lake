#ifndef ACTOR_H
#define ACTOR_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum orientation {
	CHAR_UP, CHAR_DOWN, CHAR_LEFT, CHAR_RIGHT
} Orientation;

struct location {
	int x, y;
};

struct actor {
	SDL_Texture *image;
	SDL_Rect clipping;
	struct location;
	Orientation orient;
};
void actor_set_sprite(struct actor **self);

#endif
