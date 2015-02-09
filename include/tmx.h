#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uthash.h>
#include <utarray.h>

#ifndef TILED_H
#define TILED_H


struct tile {
    unsigned gid;
    SDL_Rect source;
    UT_hash_handle hh;
};

struct tile* tile(
    const unsigned gid,
    const SDL_Rect source
);
void add_tile(struct tile **head, struct tile *tile);


struct tileset {
    SDL_Surface *image;
    struct tile *tiles;
    char *name;
    int x_pos;
    int y_pos;
    unsigned firstgid;
    unsigned tilecount;
    unsigned tilewidth;
    unsigned tileheight;
    UT_hash_handle hh;
};

struct tileset* tileset (
    unsigned firstgid,
    const char *name,
    unsigned tilewidth,
    unsigned tileheight,
    SDL_Surface *image
);
void tileset_offset(struct tileset **self, unsigned x, unsigned y);
struct tile tileset_get_tile(struct tileset **self, unsigned gid);


struct layer {
    char *name;
    double opacity;
    bool visible;
    UT_array *cells;
    UT_hash_handle hh;
};

struct layer *layer(
    const char *name,
    double opacity,
    bool visible
);
void layer_add_cells(struct layer **self, int gid);


struct tilemap {
    SDL_Surface *image;
    char *name;
    unsigned width, height;
    unsigned tilewidth, tileheight;
    struct layer *layers;
    struct tileset *tilesets;
};

struct tilemap *tilemap(
    const char *name,
    unsigned width,
    unsigned height,
    unsigned tilewidth,
    unsigned tileheight,
    Uint32 bgcolor
);
void add_layer(struct layer **head, struct layer *layer);
void add_tileset(struct tileset **head, struct tileset *tileset);
void blit_tile(struct tilemap *self,
               unsigned gid,
               SDL_Rect dest);
void blit_map(struct tilemap *self, const char *layer_name);
struct tilemap *load_tmx_json(const char *filename);

#endif
