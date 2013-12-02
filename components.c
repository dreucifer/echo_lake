#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "components.h"
#include "game.h"

#ifndef STEP_DX
#define STEP_DX 40
#endif

// Position Component Functions
component_p new_pos(SDL_Rect world_pos, enum DIRECTION dir) {
  component_p self = malloc(sizeof(component_type));
  position_p self_pos = malloc(sizeof(position_type));
  if (self == NULL) {
    fprintf(stderr, "Could Not Create Component\n");
    return NULL;
  }

  if (self_pos == NULL) {
    fprintf(stderr, "Could Not Create Position\n");
    return NULL;
  }

  self_pos->world_pos = world_pos;
  self_pos->dir = dir;

  self->delegate = self_pos;
  self->del = del_pos;

  return self;
}

void del_pos(component_p self) {
  free(self->delegate);
  free(self);
}

SDL_Rect * get_world_pos(entity_p self) {
  position_p self_pos = self->components[POSITION]->delegate;
  return &(self_pos->world_pos);
}

SDL_Rect get_screen_pos(entity_p self) {
  position_p self_pos = self->components[POSITION]->delegate;
  SDL_Rect screen_pos = self_pos->world_pos;
  
  if (!(screen_pos.x < (WIDTH / 2) - 40)) {
    screen_pos.x = (WIDTH / 2) - 40;
  }
  if (!(screen_pos.y < (HEIGHT / 2) - 40)) {
    screen_pos.y = (HEIGHT / 2) - 40;
  }
  return screen_pos;
}
enum DIRECTION get_dir(entity_p self) {
  position_p self_pos = self->components[POSITION]->delegate;
  return self_pos->dir;
}

void set_world_pos(entity_p self, SDL_Rect pos) {
  position_p self_pos = self->components[POSITION]->delegate;
  self_pos->world_pos = pos;
}

void set_dir(entity_p self, enum DIRECTION dir) {
  position_p self_pos = self->components[POSITION]->delegate;
  self_pos->dir = dir;
}
// End Position Functions

// Motion Component Functions
component_p new_motion() {
  component_p self = malloc(sizeof(component_type));
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

void set_motion(entity_p self, int vel) {
  struct motion_s *self_mot = self->components[MOTION]->delegate;

  if (get_vel(self) == 0) {
    self_mot->delta_t = STEP_DX;
    self_mot->vel = vel;
  }
}

int get_vel(entity_p self) {
  struct motion_s *self_mot = self->components[MOTION]->delegate;
  return self_mot->vel;
}

void clear_motion (entity_p self) {
  struct motion_s *self_mot = self->components[MOTION]->delegate;
  self_mot->delta_t = 0;
  self_mot->delta_c = 0;
  self_mot->vel = 0;
}

void del_motion(component_p component) {
  free(component->delegate);
  free(component);
}

component_p new_texture(const char *image_path, int w, int h) {
  component_p self = malloc(sizeof(struct component_s));
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

component_p from_texture(SDL_Texture *new_tex, int w, int h)
{
  component_p self = malloc(sizeof(struct component_s));
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

SDL_Texture* get_texture(entity_p self) {
  texture_p self_tex = self->components[TEXTURE]->delegate;
  return self_tex->texture;
}

SDL_Rect* get_section(entity_p self) {
  texture_p self_tex = self->components[TEXTURE]->delegate;
  return &(self_tex->section);
}

void set_section(entity_p self, SDL_Rect *section) {
  texture_p self_tex = self->components[TEXTURE]->delegate;
  self_tex->section = *section;
}

void del_texture(component_p component) {
  texture_p self = component->delegate;
  SDL_DestroyTexture(self->texture);
  free(self);
  free(component);
}
