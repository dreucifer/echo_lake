#include <SDL2/SDL_image.h>
#include <string.h>
#include "world.h"
#include "components.h"

struct entity *player(
		const char *name,
		const char *imagefile,
		const int w, const int h,
		SDL_Point pos,
		int direction)
{
	struct entity *self = malloc(sizeof(struct entity));
	struct player *self_data = malloc(sizeof(struct player));

	*self_data = (struct player){
		.player_name = strdup(name),
		.health = FINE,
		.sanity = COGENT,
		.action = IDLE
	};
	self = entity("player", (void **)&self_data);

	struct component *self_pos, *self_mot, *self_tex;

	self_pos = position(pos, (SDL_Point){ w, h }, (SDL_Point){ 50, 50 }, direction);
	self_mot = motion();
	self_tex = texture(imagefile, w, h);

	entity_add_component(&self, self_pos);
	entity_add_component(&self, self_mot);
	entity_add_component(&self, self_tex);

	return self;
}

struct entity *player_get() {
	return world_get_entity("player");
}

void player_destroy(struct entity **self) {
	free(*self);
	*self = NULL;
}
