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

void scroll_background(struct entity *background, struct entity *player) {
	struct texture *bg_tex = texture_from_entity(&player);
	SDL_Rect *player_pos = position_get_world(&player);
	SDL_Rect bg_pos = bg_tex->section;
	bg_pos.x = (player_pos->x > (WIDTH / 2) - 40) ?
		player_pos->x + 40 - (WIDTH / 2) : 0;
	bg_pos.y = (player_pos->y > (HEIGHT / 2) - 40) ?
		player_pos->y + 40 - (HEIGHT / 2) : 0;
	bg_pos.w = WIDTH;
	bg_pos.h = HEIGHT;
	texture_set_section(&background, &bg_pos);
}

struct entity *get_background() {
	return pool_get_entity(pool_get(), "background");
}

struct entity *get_player() {
	return pool_get_entity(pool_get(), "player");
}

int update() {
	walk_entities(pool_get()->entities);
	scroll_background(get_background(), get_player());

	return 0;
}

int render() {
	struct game *self = game_get();
	render_entities(pool_get()->entities);
	SDL_RenderPresent(self->renderer);

	return 0;
}


int main()
{
	struct game *game;
	struct entity *background, *player;
	struct pool *pool;
	SDL_Event event;
	SDL_Rect pos;
	Uint32 next_tick;
	int sleep;

	if (game_get() == NULL) {
		fprintf(stderr, "[ERROR] Could not create game singleton\n");
		return -1;
	} else {
		game = game_get();
	}
	game_get()->update = update;
	game_get()->render = render;

	atexit(SDL_Quit);

	pool = pool_get();

	pos.x = (WIDTH - 80) / 2;
	pos.y = (HEIGHT - 80) / 2;
	pos.w = 80;
	pos.h = 80;

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

	background = entity("background");
	player = entity("player");

	pool_add_entity(&pool, background);
	pool_add_entity(&pool, player);

	entity_add_component(&background,
			texture_from_texture(bg, WIDTH, HEIGHT));

	entity_add_component(&player, position(pos, DOWN));

	entity_add_component(&player, texture(
				"data/img/ranger.png", 80, 80));

	entity_add_component(&player, motion());

	pos.x = 100;
	pos.y = 100;

	sleep = 0;
	next_tick = SDL_GetTicks();

	while(!should_exit(&event)) {
		if (input(player)) {
			break;
		}

		game->update(game);

		SDL_RenderCopy(
				game_get()->renderer,
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

	game_destroy(game);
	SDL_Quit();

	return 0;
}
