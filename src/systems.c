#include <SDL2/SDL.h>
#include "game.h"
#include "systems.h"
#include "components.h"

int render_entities(entity_p entity) {
	component_p position = entity->components[POSITION];
	component_p texture = entity->components[TEXTURE];
	SDL_Rect *screen_pos = malloc(sizeof(SDL_Rect));

	if (texture != NULL && position != NULL) {
		*screen_pos = get_screen_pos(entity);
		if (screen_pos != NULL) {
			SDL_RenderCopy(
					game_singleton()->renderer,
					get_texture(entity),
					get_section(entity),
					screen_pos);
		}

	} else if (texture != NULL && position == NULL) {
		SDL_RenderCopy(
				game_singleton()->renderer,
				get_texture(entity),
				get_section(entity),
				NULL);
	}

	free(screen_pos);
	return (entity->next != NULL) ? render_entities(entity->next) : 0;
}

int walk_entities(entity_p entity) {
	position_p self_pos;
	motion_p self_mot;

	if (entity->components[POSITION] != NULL &&
			entity->components[MOTION] != NULL)  {

		self_pos =
			entity->components[POSITION]->delegate;
		self_mot =
			entity->components[MOTION]->delegate;

		switch (get_dir(entity)) {
			default:
				break;
			case UP:
				self_pos->world_pos.y += -self_mot->vel;
				break;
			case DOWN:
				self_pos->world_pos.y += self_mot->vel;
				break;
			case LEFT:
				self_pos->world_pos.x += -self_mot->vel;
				break;
			case RIGHT:
				self_pos->world_pos.x += self_mot->vel;
				break;
		}

		if (self_pos->world_pos.x < 0) {
			self_pos->world_pos.x = 0;
		}

		if (self_pos->world_pos.y < 0) {
			self_pos->world_pos.y = 0;
		}

		clear_motion(entity);
	}

	if (entity->next != NULL) {
		return walk_entities(entity->next);
	}

	return 0;
}
