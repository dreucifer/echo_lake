#include <stdlib.h>
#include <string.h>
#include "game.h"

bool handle_input(SDL_Event *event_p) {
    SDL_Event event = *(event_p);

    while(SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_QUIT:
                return true;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        printf("\nGoodbye\n");
                        break;
                    default:
                        fprintf(stderr, "\nUnhandled input\n");
                        break;
                }
                break;

            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return true;
                        break;
                    default:
                        fprintf(stderr, "\nUnhandled input\n");
                        break;
                }
                break;
        }
    }

    return false;
}

int game_update() {

    return 0;
}

int game_render() {
    game_p self = game_singleton();
    SDL_RenderPresent(self->renderer);

    return 0;
}

game_p game_constructor(const char *title) {

    game_p self = malloc(sizeof(struct game_s));
    if (self != NULL) {
        self->title = strdup(title);
        self->update = game_update;
        self->render = game_render;
    }

    return self;
}

int game_initialize(game_p self) {
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

    SDL_SetWindowTitle(self->window, self->title);

    return 0;
}

void game_destructor() {
    game_p self = game_singleton();

    SDL_DestroyRenderer(self->renderer);
    SDL_DestroyWindow(self->window);
    free(self->title);
    free(self);
}

game_p game_singleton() {
    static game_p self;

    if (self == NULL) {
        self = game_constructor(TITLE);
        game_initialize(self);
    }

    if (self->init == false) {
        game_initialize(self);
    }

    return self;
}
