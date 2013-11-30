#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "systems.h"
#include "components.h"

bool should_exit(SDL_Event *event) {
    while(SDL_PollEvent(event)) {
        switch(event->type){
            case SDL_QUIT:
                return true;
                break;
            default:
                return false;
                break;
        }
    }
    return false;
}

bool handle_input(entity_p entity) {
    static int hold_time;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_ESCAPE]) {
        printf("\nGoodbye\n");
        return true;
    }

    if (entity->components[MOTION] != NULL) {
        if (
                keystate[SDL_SCANCODE_UP] ||
                keystate[SDL_SCANCODE_DOWN] ||
                keystate[SDL_SCANCODE_LEFT] ||
                keystate[SDL_SCANCODE_RIGHT]) {
            hold_time++;
        } else {
            hold_time = 0;
        }
        if (keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_DOWN] &&
                hold_time > 3) {
            set_motion(entity, 4);
            set_dir(entity, UP);
        } 
        if (keystate[SDL_SCANCODE_DOWN] && !keystate[SDL_SCANCODE_UP] &&
                hold_time > 3) {
            set_motion(entity, 4);
            set_dir(entity, DOWN);
        }
        if (keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] &&
                hold_time > 3) {
            set_motion(entity, 4);
            set_dir(entity, LEFT);
        }
        if (keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT] &&
                hold_time > 3) {
            set_motion(entity, 4);
            set_dir(entity, RIGHT);
        }
    }

    return false;
}

void scroll_background(entity_p background, entity_p player) {
    texture_p bg_tex = background->components[TEXTURE]->delegate;
    SDL_Rect *player_pos = get_world_pos(player);
    SDL_Rect *bg_pos = &(bg_tex->section);
    bg_pos->x = (player_pos->x > (WIDTH / 2) - 40) ?
        player_pos->x + 40 - (WIDTH / 2) : 0;
    bg_pos->y = (player_pos->y > (HEIGHT / 2) - 40) ?
        player_pos->y + 40 - (HEIGHT / 2) : 0;
    bg_pos->w = WIDTH;
    bg_pos->h = HEIGHT;
    set_section(background, bg_pos);
}

entity_p get_background() {
    return find_entity("background", get_world()->entities);
}

entity_p get_player() {
    return find_entity("player", get_world()->entities);
}

int game_update() {
    walk_entities(get_world()->entities);

    scroll_background(get_background(), get_player());

    return 0;
}

int game_render() {
//    render_entities(get_world()->entities);

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
