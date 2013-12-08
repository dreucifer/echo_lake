#include <SDL2/SDL.h>
#include "game.h"
#include "systems.h"
#include "components.h"

int render_entities(struct entity *entity)
{
	struct position *position = position_from_entity(&entity);
	struct texture *texture = texture_from_entity(&entity);

	if (texture != NULL && position != NULL) {
		SDL_Texture *image = texture->image;
		SDL_Rect *source = &(texture->section);
		SDL_RenderCopy(
				game_get()->renderer,
				image,
				source,
				&(SDL_Rect){
					.x = 200,
					.y = 200,
					.w = 80,
					.h = 80
				});

	} else if (texture != NULL && position == NULL) {
		SDL_Rect source = texture_get_section(&entity);
		SDL_RenderCopy(
				game_get()->renderer,
				texture_get_image(&entity),
				&source,
				NULL);
	}

	return (entity->hh.next != NULL) ? render_entities(entity->hh.next) : 0;
}

int walk_entities(struct entity *entity)
{
	struct position *self_pos;
	struct motion *self_mot;

	if (entity_get_component(&entity, "position") != NULL &&
	    entity_get_component(&entity, "motion") != NULL)  {

		self_pos = position_from_entity(&entity);
		self_mot = motion_from_entity(&entity);

		switch (position_get_dir(&entity)) {
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

		motion_clear(&entity);
	}

	if (entity->hh.next != NULL) {
		return walk_entities(entity->hh.next);
	}

	return 0;
}
