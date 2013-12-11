#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

struct game *game(const char *title) {
	struct game *self = malloc(sizeof(struct game));
	if (self != NULL) {
		*self = (struct game){
			.window = NULL,
			.renderer = NULL,
			.world = NULL,
			.title = strdup(title),
			.init = false,
			.preload = NULL,
			.create = NULL,
			.update = NULL,
			.render = NULL,
			.destroy = NULL
		};
	}

	return self;
}

struct game *game_get() {
	static struct game *self = NULL;

	if (self == NULL) {
		self = game(TITLE);
		game_init(self);
	}

	if (self->init == false) {
		game_init(self);
	}

	return self;
}

int game_init(struct game *self) {
	self->init = true;

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "\nFailed to Initialize SDL: %s\n",
				SDL_GetError());
		self->init = false;
		return -1;
	}

	self->window = SDL_CreateWindow(self->title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WIDTH,
			HEIGHT,
			SDL_WINDOW_SHOWN);

	if(!self->window) {
		fprintf(stderr, "\nNull Window Context: %s\n",
				SDL_GetError());
		self->init = false;
		return -1;
	}

	self->renderer = SDL_CreateRenderer(self->window,
			-1, SDL_RENDERER_ACCELERATED);
	if(!self->renderer) {
		fprintf(stderr, "\nNull Render Context: %s\n",
				SDL_GetError());
		self->init = false;
		return -1;
	}

	self->world = world_get();
	if(!self->world) {
		fprintf(stderr, "[ERROR] Could not initialize world");
		return -1;
	}

	SDL_SetWindowTitle(self->window, self->title);

	return 0;
}

void game_destroy() {
	struct game *self = game_get();

	SDL_DestroyRenderer(self->renderer);
	SDL_DestroyWindow(self->window);
	free(self->title);
	free(self);
	self = NULL;
}
