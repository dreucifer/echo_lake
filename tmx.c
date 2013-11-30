#include <SDL2/SDL_image.h>
#include <string.h>
#include "tmx.h"

tile_type tile(const unsigned gid, const SDL_Rect source)
{
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
                SDL_Surface *image
) {
        unsigned height, width, max, i;
        SDL_Rect source_rect;

        height = width = max = i = 0;
        source_rect.w = tilewidth;
        source_rect.h = tileheight;
        width = image->w / tilewidth;
        height = image->h / tileheight;
        max = width * height;

        tileset_p self = (tileset_p)malloc(sizeof(tileset_type));
        tile_p tiles = (tile_p)malloc(sizeof(tile_type) * max);

        for (unsigned y = 0; y < height; y++) {
                for (unsigned x = 0; x < width; x++, i++) {
                        // note: this for loop also increments i
                        source_rect.x = x * tilewidth;
                        source_rect.y = y * tileheight;
                        tiles[i] = tile(firstgid + i, source_rect);
                }
        }

        *self = (tileset_type){
                .image = image,
                .tiles = tiles,
                .name = strdup(name),
                .firstgid = firstgid,
                .tilewidth = tilewidth,
                .tileheight = tileheight
        };

        return self;
}

void tileset_offset(tileset_p self, unsigned x, unsigned y)
{
        self->x_pos = x;
        self->y_pos = y;
}

layer_p layer(
                const char* name,
                double opacity,
                bool visible
) {
        layer_p self = (layer_p)malloc(sizeof(layer_type));
        *self = (layer_type){
                .name = strdup(name),
                .opacity = opacity,
                .visible = visible,
                .cell_ct = 0,
                .cell_mx = 1,
                .cells = (unsigned *)malloc(sizeof(unsigned *))
        };

        return self;
}

void add_cell(layer_p self, unsigned gid)
{
        if (self->cell_ct == self->cell_mx) {
                self->cell_mx *= 2;
                fprintf(stderr,
                "Not enough array for cells, expanding array to %zu\n",
                self->cell_mx);
                self->cells = (unsigned *)realloc(self->cells,
                        sizeof(unsigned *) * self->cell_mx);
        }
        self->cells[self->cell_ct++] = gid;
}

tilemap_p tilemap(
                unsigned width,
                unsigned height,
                unsigned tilewidth,
                unsigned tileheight
) {
        tilemap_p self = (tilemap_p)malloc(sizeof(tilemap_type));
        *self = (tilemap_type){
                .width = width,
                .height = height,
                .tilewidth = tilewidth,
                .tileheight = tileheight,
                .tilesets = (tileset_p *)malloc(sizeof(tileset_p)),
                .layers = (layer_p *)malloc(sizeof(layer_p)),
                .layer_ct = 0,
                .layer_mx = 1
        };

        return self;
}

void add_layer(tilemap_p self, layer_p layer, tileset_p tileset)
{
        
        if (self->layer_ct == self->layer_mx) {
                self->layer_mx += 1;
                self->layers = (layer_p *)realloc(self->layers,
                        sizeof(layer_p) * self->layer_mx);
                self->tilesets = (tileset_p *)realloc(self->tilesets,
                        sizeof(tileset_p) * self->layer_mx);
        }

        self->layers[self->layer_ct] = layer;
        self->tilesets[self->layer_ct] = tileset;
        self->layer_ct++;
}
