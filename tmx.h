#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uthash.h>
#include <utarray.h>
#include "macros.h"

#ifndef TILED_H
#define TILED_H


classdef(tile);

void add_tile(tile_p *head, tile_p tile);

tile_p tile(
        unsigned gid,
        SDL_Rect source
);

struct tile_s {
        unsigned gid;
        SDL_Rect source;
        UT_hash_handle hh;
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
tile_type tileset_get_tile(tileset_p self, unsigned gid);

struct tileset_s {
        SDL_Surface *image;
        tile_p tiles;
        char* name;
        int x_pos;
        int y_pos;
        unsigned firstgid;
        unsigned tilecount;
        unsigned tilewidth;
        unsigned tileheight;
        UT_hash_handle hh;
};


classdef(layer);

layer_p layer(
        const char* name,
        double opacity,
        bool visible
);
void layer_add_cells(layer_p self, int gid);

struct layer_s {
        char* name;
        double opacity;
        bool visible;
        UT_array *cells;
        UT_hash_handle hh;
};


classdef(tilemap);

tilemap_p tilemap(
        const char* name,
        unsigned width,
        unsigned height,
        unsigned tilewidth,
        unsigned tileheight,
        Uint32 bgcolor
);
void add_layer(layer_p *head, layer_p layer);
void add_tileset(tileset_p *head, tileset_p tileset);
void blit_tile(tilemap_p self,
        unsigned gid,
        SDL_Rect dest);
void blit_map(tilemap_p self, const char* layer_name);
tilemap_p load_tmx_json(const char* filename);

struct tilemap_s {
        SDL_Surface *image;
        char* name;
        unsigned width, height;
        unsigned tilewidth, tileheight;
        layer_p layers;
        tileset_p tilesets;
};

#endif
