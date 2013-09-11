#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "entity.h"
#include "components.h"
#include "systems.h"

const int SKIP_TICKS = 1000 / FPS;

int main(int argc, char *argv[]) {

    game_p game;
    entity_p background, player, obstacle;
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

    get_world();

    background = new_entity("background");
    obstacle = new_entity("obstacle");
    player = new_entity("actor");

    register_component(background, TEXTURE,
            new_texture("sprites/background.png", WIDTH, HEIGHT));

    register_component(player, POSITION,
            new_position((WIDTH - 80) / 2, (HEIGHT - 80) / 2,
                80, 80, DOWN));

    register_component(player, TEXTURE,
            new_texture("sprites/ranger.png",
                80, 80));

    register_component(player, MOTION,
            new_motion());

    register_component(obstacle, POSITION,
            new_position(100, 100, 80, 80, DOWN));

    register_component(obstacle, TEXTURE,
            new_texture("sprites/ranger.png",
                80, 80));

    sleep = 0;
    next_tick = SDL_GetTicks();

    while(!should_exit(&event)) {
        if (handle_input(player)) {
            break;
        }

        game->update(game);

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

    game_destructor(game);
    SDL_Quit();

    return 0;
}


