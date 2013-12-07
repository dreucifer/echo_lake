#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "components.h"
#include "game.h"

#ifndef STEP_DX
#define STEP_DX 40
#endif

// Position Component Functions
struct component *position(SDL_Rect world_pos, enum DIRECTION dir) {
	struct component *self = (struct component *)malloc(sizeof(struct component));
	struct position *self_pos = (struct position *)malloc(sizeof(struct position));
	if (self == NULL) {
		fprintf(stderr, "Could Not Create Component\n");
		return NULL;
	}
	if (self_pos == NULL) {
		fprintf(stderr, "Could Not Create Position\n");
		return NULL;
	}

	*self_pos = (struct position){
		.world_pos = world_pos,
			.dir = dir
	};

	*self = (struct component){
		.delegate = self_pos
	};

	return self;
}

void position_destroy(struct component *self) {
	free(self->delegate);
	free(self);
}

SDL_Rect position_get_world(struct entity *self) {
	struct position *self_pos = component_get("position", &self);
	return (*self_pos).world_pos;
}

SDL_Rect position_get_screen(struct entity *self) {
	struct position *self_pos = self->components[POSITION]->delegate;
	SDL_Rect screen_pos = self_pos->world_pos;

	if (!(screen_pos.x < (WIDTH / 2) - 40)) {
		screen_pos.x = (WIDTH / 2) - 40;
	}
	if (!(screen_pos.y < (HEIGHT / 2) - 40)) {
		screen_pos.y = (HEIGHT / 2) - 40;
	}
	return screen_pos;
}
enum DIRECTION get_dir(struct entity *self) {
	struct position *self_pos = self->components[POSITION]->delegate;
	return self_pos->dir;
}

void set_world_pos(struct entity *self, SDL_Rect pos) {
	struct position *self_pos = self->components[POSITION]->delegate;
	self_pos->world_pos = pos;
}

void set_dir(struct entity *self, enum DIRECTION dir) {
	struct position *self_pos = self->components[POSITION]->delegate;
	self_pos->dir = dir;
}
// End Position Functions

// Motion Component Functions
struct component *new_motion() {
	struct component *self = malloc(sizeof(struct component));
	struct motion_s *self_mot = malloc(sizeof(struct motion_s));
	if (self == NULL) {
		fprintf(stderr, "Could Not Create Component\n");
		return NULL;
	}

	if (self_mot == NULL) {
		fprintf(stderr, "Could Not Create Motion\n");
		return NULL;
	}

	self_mot->delta_t = 0;
	self_mot->delta_c = 0;
	self_mot->vel = 0;

	self->delegate = self_mot;
	self->del = del_motion;

	return self;
}

void set_motion(struct entity *self, int vel) {
	struct motion_s *self_mot = self->components[MOTION]->delegate;

	if (get_vel(self) == 0) {
		self_mot->delta_t = STEP_DX;
		self_mot->vel = vel;
	}
}

int get_vel(struct entity *self) {
	struct motion_s *self_mot = self->components[MOTION]->delegate;
	return self_mot->vel;
}

void clear_motion (struct entity *self) {
	struct motion_s *self_mot = self->components[MOTION]->delegate;
	self_mot->delta_t = 0;
	self_mot->delta_c = 0;
	self_mot->vel = 0;
}

void del_motion(struct component *component) {
	free(component->delegate);
	free(component);
}

struct component *new_texture(const char *image_path, int w, int h) {
	struct component *self = malloc(sizeof(struct component_s));
	texture_p self_tex = malloc(sizeof(struct texture_s));
	SDL_Texture *new_tex;
	SDL_Surface *loaded_surface = IMG_Load(image_path);

	if (loaded_surface == NULL) {
		fprintf(stderr, "Unable to load image: %s! SDL_Image error: %s\n",
				image_path, IMG_GetError());
		return NULL;
	} else {
		new_tex = SDL_CreateTextureFromSurface(game_singleton()->renderer,
				loaded_surface);
	}

	self_tex->texture = new_tex;
	self_tex->section = (SDL_Rect){
		.x = 0,
			.y = 0,
			.w = w,
			.h = h
	};

	self->delegate = self_tex;
	self->del = del_texture;

	SDL_FreeSurface(loaded_surface);

	return self;
}

struct component *from_texture(SDL_Texture *new_tex, int w, int h)
{
	struct component *self = malloc(sizeof(struct component_s));
	texture_p self_tex = malloc(sizeof(struct texture_s));

	self_tex->texture = new_tex;
	self_tex->section = (SDL_Rect){
		.x = 0,
			.y = 0,
			.w = w,
			.h = h
	};

	self->delegate = self_tex;
	self->del = del_texture;

	return self;
}

SDL_Texture* get_texture(struct entity *self) {
	texture_p self_tex = self->components[TEXTURE]->delegate;
	return self_tex->texture;
}

SDL_Rect* get_section(struct entity *self) {
	texture_p self_tex = self->components[TEXTURE]->delegate;
	return &(self_tex->section);
}

void set_section(struct entity *self, SDL_Rect *section) {
	texture_p self_tex = self->components[TEXTURE]->delegate;
	self_tex->section = *section;
}

void del_texture(struct component *component) {
	texture_p self = component->delegate;
	SDL_DestroyTexture(self->texture);
	free(self);
	free(component);
}
