#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "entity.h"
#include "position.h"
#include "texture.h"

const int SKIP_TICKS = 1000 / FPS;

int main(int argc, char *argv[]) {

    world_p world;
    game_p game;
    if (game_singleton() == NULL) {
        fprintf(stderr, "Could not create game singleton\n");
        return -1;
    } else {
        game = game_singleton();
    }

    entity_p player;
    SDL_Texture *background;
    SDL_Surface *temp;
    SDL_Rect bg_dest;
    SDL_Event event;
    bool doexit = false;
    Uint32 next_tick;
    int sleep;

    atexit(SDL_Quit);

    bg_dest = (SDL_Rect) { .x = 0, .y = 8, .w = WIDTH, .h = HEIGHT };
    temp = IMG_Load("sprites/background.png");
    background = SDL_CreateTextureFromSurface(game->renderer, temp);
    SDL_FreeSurface(temp);

    world = get_world();

    player = new_entity(world);

    register_component(player, POSITION,
            new_position((WIDTH - 80) / 2, (HEIGHT - 80) / 2,
                80, 80, CH_DOWN));

    register_component(player, TEXTURE,
            new_texture("sprites/ranger.png",
                80, 80));

    sleep = 0;
    next_tick = SDL_GetTicks();

    while(!doexit) {
        doexit = handle_input(&event);

        SDL_RenderCopy(game->renderer, background, &bg_dest, NULL);

        render_texture(player->components[TEXTURE],
                get_position(player->components[POSITION]));

        game->render(game);

        next_tick += SKIP_TICKS;
        sleep = next_tick - SDL_GetTicks();
        if (sleep > 0) {
            SDL_Delay(sleep);
        }
    }

    SDL_DestroyTexture(background);
    destroy_position(player->components[POSITION]);
    destroy_texture(player->components[TEXTURE]);
    free(player);
    game_destructor(game);
    SDL_Quit();

    return 0;
}


