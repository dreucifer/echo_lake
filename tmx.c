#include <SDL2/SDL_image.h>
#include <string.h>
#include "tmx.h"

tile_type tile(const unsigned gid, const SDL_Rect source) {
  tile_type tile;
  tile.gid = gid;
  tile.source = source;
  return tile;
}

tileset_p tileset(
    unsigned firstgid,
    const char* name,
    unsigned tilewidth,
    unsigned tileheight,
    unsigned spacing,
    unsigned margin,
    SDL_Surface *image) {

  tileset_p self = (tileset_p)malloc(sizeof(tileset_type));
  SDL_Rect source_rect;
  source_rect.w = tilewidth;
  source_rect.h = tileheight;
  unsigned height, width, max, i;
  i = 0;
  width = image->w / tilewidth;
  height = image->h / tileheight;
  max = width * height;
  tile_p tiles = (tile_p)malloc(sizeof(tile_type) * (max + 1));
  for (unsigned y = 0; y < height; y++) {
    for (unsigned x = 0; x < width; x++) {
      source_rect.x = x * tilewidth;
      source_rect.y = y * tileheight;
      tiles[i] = tile(firstgid + i, source_rect);
      i++;
    }
  }

  self->firstgid = firstgid;
  self->name = strdup(name);
  self->tilewidth = tilewidth;
  self->tileheight = tileheight;
  self->spacing = spacing;
  self->margin = margin;
  self->tiles = tiles;
  self->image = image;
  return self;
}

void del_tileset(tileset_p self) {
  free(self->tiles);
  free(self);
  self = NULL;
}

void tileset_offset(tileset_p self, unsigned x, unsigned y) {
  self->x_pos = x;
  self->y_pos = y;
}

layer_p layer(const char* name, double opacity, bool visible) {
  layer_p self = (layer_p)malloc(sizeof(layer_type));
  self->name = strdup(name);
  self->opacity = opacity;
  self->visible = visible;
  return self;
}
