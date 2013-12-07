#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "actor.h"

extern const int STEP_DX;

typedef enum FRAME {
	STOPPED, IDLE, RIGHT_FOOT, LEFT_FOOT
} Frame;

typedef enum HEALTH {
	DEAD, DANGER, CAUTION, HURT, FINE
} Health;

typedef enum SANITY {
	GONE, PSYCHOTIC, DELUSIONAL, PARANOID, COGENT
} Sanity;

typedef enum ACTION {
	STOP, WALK, ATTACK, INSPECT
} Action;

struct player {
	/***** Character Attributes *****
	 * image:
	 * image_default:
	 * clipping: clipping rectangle
	 * location: location rectangle
	 * orient:
	 * hold_time:
	 * walk_state:
	 * dx:
	 * frame:
	 * set_sprite():
	 ********************************/
	struct actor;
	Health health;
	Sanity sanity;
	Action action;
	Frame frame;
	int hold_time, frame_time;
	int dx;
};
struct player *player(
		const char *imagefile,
		const int loc_x,
		const int loc_y,
		Orientation orientation,
		SDL_Renderer *renderer);
int player_update(struct player *self, int dt, bool *key);
void player_render(struct player *self, SDL_Renderer *renderer);
void player_destroy(struct player *self);

#endif
