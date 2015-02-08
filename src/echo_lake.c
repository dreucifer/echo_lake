#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "statemanager.h"
#include "entity.h"
#include "components.h"
#include "systems.h"
#include "tmx.h"
#include "world.h"

const int SKIP_TICKS = 1000 / FPS;

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

bool input(struct entity *entity) {
	static int hold_time;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE]) {
		printf("[INFO] Escaped pressed, shutting down.\n");
		return true;
	}

	if (motion_from_entity(&entity) != NULL) {
		if (keystate[SDL_SCANCODE_UP] ||
				keystate[SDL_SCANCODE_DOWN] ||
				keystate[SDL_SCANCODE_LEFT] ||
				keystate[SDL_SCANCODE_RIGHT]) {
			hold_time++;
		} else {
			hold_time = 0;
		}
		if (keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_DOWN] &&
				hold_time > 3) {
			motion_set(&entity, 4);
			position_set_dir(&entity, UP);
		} 
		if (keystate[SDL_SCANCODE_DOWN] && !keystate[SDL_SCANCODE_UP] &&
				hold_time > 3) {
			motion_set(&entity, 4);
			position_set_dir(&entity, DOWN);
		}
		if (keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] &&
				hold_time > 3) {
			motion_set(&entity, 4);
			position_set_dir(&entity, LEFT);
		}
		if (keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT] &&
				hold_time > 3) {
			motion_set(&entity, 4);
			position_set_dir(&entity, RIGHT);
		}
	}

	return false;
}

int update() {
	walk_entities(&world_get()->entities);

	return 0;
}

int render() {
	world_render(&world_get()->entities);
	SDL_RenderPresent(game_get()->renderer);

	return 0;
}


int main()
{
	struct game *game;
	struct entity *tilemap, *ranger;
	SDL_Event event;
	Uint32 next_tick;
	int sleep;
	SDL_Point world_pos;

	if (game_get() == NULL) {
		fprintf(stderr, "[ERROR] Could not create game singleton\n");
		return -1;
	} else {
		game = game_get();
	}

	world_pos = (SDL_Point){ 1200, 1200 };
	world_get()->bounds = world_pos;
	game_get()->update = update;
	game_get()->render = render;

	atexit(SDL_Quit);

	SDL_Surface *temp;
	temp = IMG_Load("/home/dreucifer/GameDev/echo_lake/data/img/tiles/mansionset.png");
	if (!temp) {
		fprintf(stderr, "[ERROR] cannot load image: %s\n",
				IMG_GetError());
		return 0;
	}
	struct tilemap *tilemap01 = load_tmx_json("data/maps/level1.json");
	blit_map(tilemap01, "ground");
	blit_map(tilemap01, "buildings");

	SDL_Texture *bg;
	bg = SDL_CreateTextureFromSurface(
			game_get()->renderer,
			tilemap01->image
			);

	world_pos = (SDL_Point){ 600, 1000 };

	ranger = player("ranger", "data/img/ranger.png", 80, 80,
			world_pos, DOWN);
	tilemap = entity("tilemap", NULL);

	world_add_entity(tilemap);
	world_add_entity(ranger);

	entity_add_component(&tilemap,
			texture_from_texture(bg, WIDTH, HEIGHT));

	sleep = 0;
	next_tick = SDL_GetTicks();

	while(!should_exit(&event)) {
		if (input(ranger)) {
			break;
		}

		game->update();
		camera_follow(&ranger);

		if (game->render() != 0) {
			fprintf(stderr, "[ERROR] Render failure!!\n");
			break;
		}

		next_tick += SKIP_TICKS;
		sleep = next_tick - SDL_GetTicks();
		if (sleep > 0) {
			SDL_Delay(sleep);
		}
	}

	game_destroy(game);
	SDL_Quit();

	return 0;
}
