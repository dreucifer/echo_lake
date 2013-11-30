#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "macros.h"

#ifndef TILED_H
#define TILED_H


classdef(tile);

tile_type new_tile(
        const unsigned gid,
        const SDL_Rect source
);

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
        SDL_Surface *image
);
void tileset_offset(tileset_p self, unsigned x, unsigned y);

struct tileset_s {
        SDL_Surface *image;
        tile_type *tiles;
        char* name;
        int x_pos;
        int y_pos;
        unsigned firstgid;
        unsigned tilewidth;
        unsigned tileheight;
};


classdef(layer);

layer_p layer(
        const char* name,
        double opacity,
        bool visible
);
void add_cell(layer_p self, unsigned gid);

struct layer_s {
        char* name;
        double opacity;
        bool visible;
        unsigned *cells;
        size_t cell_ct;
        size_t cell_mx;
};


classdef(tilemap);

tilemap_p tilemap(
        unsigned width,
        unsigned height,
        unsigned tilewidth,
        unsigned tileheight
);
void add_layer(tilemap_p self, layer_p layer, tileset_p tileset);

struct tilemap_s {
        SDL_Surface *image;
        unsigned width, height;
        unsigned tilewidth, tileheight;
        tileset_p *tilesets;
        layer_p *layers;
        size_t layer_ct;
        size_t layer_mx;
};

#endif
