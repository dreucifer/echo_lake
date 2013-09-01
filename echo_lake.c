#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "entity.h"
#include "components.h"
#include "systems.h"

const int SKIP_TICKS = 1000 / FPS;

int main(int argc, char *argv[]) {

    world_p world;
    game_p game;
    entity_p player, player2;
    SDL_Texture *background;
    SDL_Surface *temp;
    SDL_Rect bg_dest;
    SDL_Event event;
    Uint32 next_tick;
    int sleep;

    if (game_singleton() == NULL) {
        fprintf(stderr, "Could not create game singleton\n");
        return -1;
    } else {
        game = game_singleton();
    }

    atexit(SDL_Quit);

    bg_dest = (SDL_Rect) { .x = 0, .y = 8, .w = WIDTH, .h = HEIGHT };
    temp = IMG_Load("sprites/background.png");
    background = SDL_CreateTextureFromSurface(game->renderer, temp);
    SDL_FreeSurface(temp);

    world = get_world();

    player = new_entity(world);
    player2 = new_entity(world);

    register_component(player, POSITION,
            new_position((WIDTH - 80) / 2, (HEIGHT - 80) / 2,
                80, 80, DOWN));

    register_component(player, TEXTURE,
            new_texture("sprites/ranger.png",
                80, 80));

    register_component(player, MOTION,
            new_motion());

    register_component(player2, POSITION,
            new_position(100, 100, 80, 80, DOWN));

    register_component(player2, TEXTURE,
            new_texture("sprites/ranger.png",
                80, 80));

    sleep = 0;
    next_tick = SDL_GetTicks();

    while(!should_exit(&event)) {
        if (handle_input(player)) {
            break;
        }
        game->update(game);

        SDL_RenderCopy(game->renderer, background, &bg_dest, NULL);

        if (game->render(game) != 0) {
            fprintf(stderr, "Render failure!!\n");
            break;
        }

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


