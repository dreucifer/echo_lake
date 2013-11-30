#include <SDL2/SDL.h>
#include <stdbool.h>
#include "macros.h"

#ifndef TILED_H
#define TILED_H

classdef(tile);

tile_type new_tile(const unsigned gid, const SDL_Rect source);

struct tile_s {
  unsigned gid;
  SDL_Rect source;
};

classdef(tileset);

tileset_p tileset (
    unsigned firstgid,
    const char* name,
    unsigned tilewidth,
    unsigned tileheight,
    unsigned spacing,
    unsigned margin,
    SDL_Surface *image);
void del_tileset(tileset_p self);
void tileset_offset(tileset_p self, unsigned x, unsigned y);

struct tileset_s {
  void (*offset)(tileset_type self, unsigned x, unsigned y);

  unsigned firstgid;
  char* name;
  unsigned tilewidth;
  unsigned tileheight;
  unsigned spacing;
  unsigned margin;

  int x_pos;
  int y_pos;

  SDL_Surface *image;

  tile_type *tiles;
};

classdef(layer);

struct layer_s {
  char* name;
  double opacity;
  bool visible;

  unsigned *tile_gids;
};

layer_p layer(const char* name, double opacity, bool visible);

classdef(tilemap);

struct tilemap_s {
  tileset_p *tilesets;
  layer_p *layers;

};

#endif
