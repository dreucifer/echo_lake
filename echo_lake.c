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

  SDL_Surface *tiles_img;
  tiles_img = IMG_Load("img/tiles/mansionset.png");
  if (tiles_img == NULL) {
    fprintf(stderr, "Unable to load image: %s! SDL_Image error: %s\n", "img/tiles/mansionset.png", IMG_GetError());
  }

  tileset_p tileset01 = tileset(
      5, "background", 80, 80, 0, 0, tiles_img
      );


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

    if (game->render(game) != 0) {
      fprintf(stderr, "Render failure!!\n");
      break;
    }

    SDL_RenderCopy(
        game->renderer,
        SDL_CreateTextureFromSurface(game->renderer, tileset01->image),
        &tileset01->tiles[5].source,
        &tileset01->tiles[0].source);

    next_tick += SKIP_TICKS;
    sleep = next_tick - SDL_GetTicks();
    if (sleep > 0) {
      SDL_Delay(sleep);
    }
  }

  game_destructor(game);
  del_tileset(tileset01);
  SDL_Quit();

  return 0;
}


