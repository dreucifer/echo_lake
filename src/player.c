#include <SDL2/SDL_image.h>
#include <string.h>
#include "player.h"

const int STEP_DX = 40;

void set_sprite(void * self_void)
{
	struct player *self = self_void;

	switch(self->frame) {
		case RIGHT_FOOT:
			self->clipping.x = 160; 
			break;
		case LEFT_FOOT:
			self->clipping.x = 240; 
			break;
		case IDLE:
			self->clipping.x = 80;
			break;
		case STOPPED:
			self->clipping.x = 0;
			break;
		default:
			self->clipping.x = 0;
			break;
	}

	switch(self->orient) {
		case CHAR_UP:
			self->clipping.y = 80;
			break;
		case CHAR_DOWN:
			self->clipping.y = 0;
			break;
		case CHAR_LEFT:
			self->clipping.y = 240;
			break;
		case CHAR_RIGHT:
			self->clipping.y = 160;
			break;
		default:
			self->clipping.y = 0;
			break;
	}

}


int player_update(struct player *self, int dt, _Bool *key)
{


	/* Increment holdtime and set orientation based on button */
	if(key[0]) {
		if (self->action != WALK) {
			if (self->orient != CHAR_UP) {
				self->orient = CHAR_UP;
				set_sprite(self);
			}

			self->hold_time += dt;
		}

	} else if(key[1]) {
		if (self->action != WALK) {
			if (self->orient != CHAR_DOWN) {
				self->orient = CHAR_DOWN;
				set_sprite(self);
			}

			self->hold_time += dt;
		}

	} else if(key[2]) {
		if (self->action != WALK) {
			if (self->orient != CHAR_LEFT) {
				self->orient = CHAR_LEFT;
				set_sprite(self);
			}

			self->hold_time += dt;
		}

	} else if(key[3]) {
		if (self->action != WALK) {
			if (self->orient != CHAR_RIGHT) {
				self->orient = CHAR_RIGHT;
				set_sprite(self);
			}

			self->hold_time += dt;
		}

	} else {
		self->hold_time = 0;
	}

	/* Switch to walking if button is held for 0.1 seconds */
	if (self->hold_time >= 100) {
		self->action = WALK;
	} 

	/* Move character 8 pixels at a time in the direction *
	 * they are facing */
	if (self->action == WALK && self->dx < STEP_DX) {

		self->dx += 4;
		/*    switch (self->orient) {
		      case CHAR_UP:
		      self->y -= 4;
		      break;
		      case CHAR_DOWN:
		      self->y += 4;
		      break;
		      case CHAR_LEFT:
		      self->x -= 4;
		      break;
		      case CHAR_RIGHT:
		      self->x += 4;
		      break;
		      default:
		      break;
		      }
		      */  }

	if (self->frame_time >= 100) {
		if (self->action == WALK) {
			if (self->frame != RIGHT_FOOT) {
				self->frame = RIGHT_FOOT;
			} else if (self->frame != LEFT_FOOT) {
				self->frame = LEFT_FOOT;
			}
		}

		if (!self->action == WALK) {
			if (self->frame != STOPPED) {
				self->frame = STOPPED;
			} else if (self->frame != IDLE) {
				self->frame = IDLE;
			}
		}

		self->frame_time = 0;
	}

	if (self->dx == STEP_DX) {
		self->action = STOP;
		self->dx = 0;
		if (self->hold_time == 0) self->frame = STOPPED;
	}

	self->frame_time += dt;
	set_sprite(self);

	return 0;
}


void player_render(struct player *self, SDL_Renderer *renderer)
{
	SDL_Rect temp;
	temp.x = self->x;
	temp.y = self->y;
	temp.w = 80;
	temp.h = 80;

	SDL_RenderCopy(renderer, self->image, &(self->clipping), &temp);
}



struct player *player(
		const char *imagefile,
		const int loc_x,
		const int loc_y,
		Orientation orientation,
		SDL_Renderer *renderer)
{
	/* This function is a factory for the player pseudoclass *
	 * it creates a permanent instance of a player struct,   *
	 * initializes the values, and initializes the function  *
	 * pointers.                                             */


	struct player *self = malloc(sizeof(struct player));
	SDL_Surface *temp;

	self->orient      = orientation;
	self->action      = STOP;
	self->frame       = STOPPED;
	self->health      = FINE;
	self->sanity      = COGENT;
	self->frame_time  = 0;
	self->hold_time   = 0;
	self->dx          = 0;
	self->clipping    = (SDL_Rect){ .w = 80, .h = 80 };

	self->x = loc_x;
	self->y = loc_y;

	/* Initalize the spritemap (image) from file, then creat  *
	 * an empty render surface, which is filled by set_sprite */
	temp = IMG_Load(imagefile);
	self->image = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	set_sprite(self);

	return self;
}


void player_destroy(struct player *self) {
	SDL_DestroyTexture(self->image);
	free(self);
	self = NULL;
}
