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

int main()
{
        game_p game;
        entity_p background, player, obstacle;
        SDL_Event event;
        SDL_Rect position;
        Uint32 next_tick;
        int sleep;

        if (game_singleton() == NULL) {
                fprintf(stderr, "[ERROR] Could not create game singleton\n");
                return -1;
        } else {
                game = game_singleton();
        }

        atexit(SDL_Quit);

        get_world();

        background = new_entity("background");
        obstacle = new_entity("obstacle");
        player = new_entity("player");

        position.x = (WIDTH - 80) / 2;
        position.y = (HEIGHT - 80) / 2;
        position.w = 80;
        position.h = 80;

        tileset_p tileset01 = tileset(1, "bg", 80, 80,
                        IMG_Load("img/tiles/mansionset.png"));

        layer_p layer01 = layer("bg", 1.0, true);
        layer_p layer02 = layer("fg", 1.0, true);

        int data1[] = {
                3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
        };
        int data2[] = {
                12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
                        1,2,1,2,1,2,1,9,2,1,2,1,2,1,2,
                        10,11,10,11,10,11,10,12,11,10,11,10,11,10,11,
                        13,14,13,14,13,14,13,15,14,13,14,13,14,13,14,
                        1,2,1,2,1,2,1,9,2,1,2,1,2,1,2,
                        10,11,10,11,10,11,10,12,11,10,11,10,11,10,11,
                        13,14,13,14,13,14,13,15,14,13,14,13,14,13,14,
                        4,5,4,5,4,5,4,9,5,4,5,4,5,4,5,
                        7,8,7,8,7,8,7,0,8,7,8,7,8,7,8,
                        10,11,10,11,10,11,7,0,8,10,11,10,11,10,11,
                        13,14,13,14,13,14,10,12,11,13,14,13,14,13,14,
                        0,0,0,0,0,16,17,17,17,18,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };

        int count = sizeof(data1) / sizeof(unsigned);
        for (int i = 0; i < count; i++) {
                layer_add_cells(layer01, data1[i]);
        }

        for (int i = 0; i < count; i++) {
                layer_add_cells(layer02, data2[i]);
        }

        tilemap_p tilemap02 = load_tmx_json("maps/level1.json");
        add_layer(&(tilemap02->layers), layer01);
        add_layer(&(tilemap02->layers), layer02);
        add_tileset(&(tilemap02->tilesets), tileset01);

        blit_map(tilemap02, "bg");
        blit_map(tilemap02, "fg");

        SDL_Texture *bg;
        bg = SDL_CreateTextureFromSurface(
                        game_singleton()->renderer,
                        tilemap02->image
                        );

        register_component(background, TEXTURE,
                        from_texture(bg, WIDTH, HEIGHT));

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

                SDL_RenderCopy(
                                game_singleton()->renderer,
                                bg, NULL, &(SDL_Rect){
                                        .x = 0,
                                        .y = 0,
                                        .w = 1280,
                                        .h = 1280
                                }
                              );

                if (game->render(game) != 0) {
                        fprintf(stderr, "[ERROR] Render failure!!\n");
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
