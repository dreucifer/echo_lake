#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

static void cleanup()
{
    struct game *self = game();

    SDL_DestroyRenderer(self->renderer);
    SDL_DestroyWindow(self->window);
    free(self->title);
    free(self);
    self = NULL;
}

static int render() {
    struct state *cur_state = game()->statemanager->head->data;
    cur_state->render();
    SDL_RenderPresent(game()->renderer);

    return 0;
}

static int update() {
    return 0;
}

struct game *game()
{
    static struct game *self = NULL;

    if (self == NULL) {
        self = malloc(sizeof(struct game));
        self->window = NULL;
        self->renderer = NULL;
        self->world = NULL;
        self->title = strdup(TITLE);
        self->init = false;
        self->preload = NULL;
        self->create = NULL;
        self->update = NULL;
        self->render = NULL;
        self->cleanup = NULL;
        self->entitymanager = NULL;
        self->statemanager = NULL;

        game_init(self);
    }

    if (self->init == false) {
        game_init(self);
    }

    return self;
}

int game_init(struct game *self)
{
    self->init = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "\nFailed to Initialize SDL: %s\n",
                SDL_GetError());
        self->init = false;
        return -1;
    }

    self->window = SDL_CreateWindow(self->title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!self->window) {
        fprintf(stderr, "\nNull Window Context: %s\n", SDL_GetError());
        self->init = false;
        return -1;
    }

    self->renderer = SDL_CreateRenderer(self->window,
                                        -1, SDL_RENDERER_ACCELERATED);
    if (!self->renderer) {
        fprintf(stderr, "\nNull Render Context: %s\n", SDL_GetError());
        self->init = false;
        return -1;
    }

    self->world = world_get();
    if (!self->world) {
        fprintf(stderr, "[ERROR] Could not initialize world");
        return -1;
    }

    self->entitymanager = entitymanager();
    if (!self->entitymanager) {
        fprintf(stderr, "[ERROR] Could not initialize entity manager");
        return -1;
    }

    self->statemanager = statemanager();
    if (!self->statemanager) {
        fprintf(stderr, "[ERROR] Could not initialize state manager");
        return -1;
    }

    SDL_SetWindowTitle(self->window, self->title);

    self->cleanup = &cleanup;
    self->render = &render;
    self->update = &update;

    return 0;
}
