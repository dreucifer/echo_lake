#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "game.h"
#include "entity.h"
#include "components.h"
#include "systems.h"
#include "tmx.h"

const int SKIP_TICKS = 1000 / FPS;

int main(int argc, char *argv[]) {
        game_p game;
        entity_p background, player, obstacle;
        SDL_Event event;
        SDL_Rect position;
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
        player = new_entity("player");

        register_component(background, TEXTURE,
                        new_texture("img/background.png", WIDTH, HEIGHT));

        position.x = (WIDTH - 80) / 2;
        position.y = (HEIGHT - 80) / 2;
        position.w = 80;
        position.h = 80;

        tileset_p tileset01 = tileset(0, "bg", 80, 80,
                        IMG_Load("img/tiles/mansionset.png"));

        layer_p layer01 = layer("bg", 1.0, true);

        unsigned data[] = {
                12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 1, 2, 1, 2, 1, 2, 1, 9, 2, 1, 2, 1, 2, 1, 2, 10, 11, 10, 11, 10, 11, 10, 12, 11, 10, 11, 10, 11, 10, 11, 13, 14, 13, 14, 13, 14, 13, 15, 14, 13, 14, 13, 14, 13, 14, 1, 2, 1, 2, 1, 2, 1, 9, 2, 1, 2, 1, 2, 1, 2, 10, 11, 10, 11, 10, 11, 10, 12, 11, 10, 11, 10, 11, 10, 11, 13, 14, 13, 14, 13, 14, 13, 15, 14, 13, 14, 13, 14, 13, 14, 4, 5, 4, 5, 4, 5, 4, 9, 5, 4, 5, 4, 5, 4, 5, 7, 8, 7, 8, 7, 8, 7, 0, 8, 7, 8, 7, 8, 7, 8, 3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 6, 0, 3, 14, 13, 3, 3, 14, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
        };

        int count = sizeof(data) / sizeof(unsigned);
        for (int i = 0; i < count; i++) {
                add_cell(layer01, data[i]);
        }

        tilemap_p tilemap01 = tilemap(
                        15, 15, 80, 80);

        add_layer(tilemap01, layer01, tileset01);

        SDL_Texture *bg;

        SDL_BlitSurface(
                tilemap01->tilesets[0]->image,
                NULL,
                tilemap01->image,
                &(SDL_Rect){
                        .x = 0,
                        .y = 0,
                        .w = 80,
                        .h = 80
                }
        );

        register_component(player, POSITION,
                        new_pos(position, DOWN));

        register_component(player, TEXTURE,
                        new_texture("img/ranger.png",
                                80, 80));

        register_component(player, MOTION,
                        new_motion());

        position.x = 100;
        position.y = 100;

        register_component(obstacle, POSITION,
                        new_pos(position, DOWN));

        register_component(obstacle, TEXTURE,
                        new_texture("img/ranger.png",
                                80, 80));

        sleep = 0;
        next_tick = SDL_GetTicks();

        while(!should_exit(&event)) {
                if (handle_input(player)) {
                        break;
                }

                game->update(game);

                bg = SDL_CreateTextureFromSurface(
                                game_singleton()->renderer,
                                tilemap01->image
                                );
                SDL_RenderCopy(
                                game_singleton()->renderer,
                                bg, NULL, NULL
                              );

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
