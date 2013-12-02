#include <SDL2/SDL_image.h>
#include <assert.h>
#include <string.h>
#include <jansson.h>
#include "tmx.h"

tile_p tile(const unsigned gid, const SDL_Rect source)
{
    tile_p self = (tile_p)malloc(sizeof(tile_type));
    self->gid = gid;
    self->source = source;

    return self;
}

void add_tile(tile_p *head, tile_p tile)
{
    HASH_ADD_INT(*head, gid, tile);
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
    tile_p tiles = NULL;

    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++, i++) {
            // note: this for loop also increments i
            source_rect.x = x * tilewidth;
            source_rect.y = y * tileheight;
            add_tile(&tiles, tile(firstgid + i, source_rect));
        }
    }

    *self = (tileset_type){
        .image = image,
            .tiles = tiles,
            .name = strdup(name),
            .firstgid = firstgid,
            .tilecount = max,
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
            .visible = visible
    };
    utarray_new(self->cells, &ut_int_icd);

    return self;
}

void layer_add_cells(layer_p self, int gid)
{
    utarray_push_back(self->cells, &gid);
}

unsigned calc_width(unsigned width_ct, unsigned tilewidth)
{
    return width_ct * tilewidth;
}

unsigned calc_height(unsigned height_ct, unsigned tileheight)
{
    return height_ct * tileheight;
}

tilemap_p tilemap(
        const char* name,
        unsigned width,
        unsigned height,
        unsigned tilewidth,
        unsigned tileheight,
        Uint32 bgcolor
        ) {
    SDL_Surface *image;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     * on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    unsigned width_px = calc_width(width, tilewidth);
    unsigned height_px = calc_height(height, tileheight);
    image = SDL_CreateRGBSurface(0, width_px, height_px, 32,
            rmask, gmask, bmask, amask);
    SDL_FillRect(image, NULL, bgcolor);

    tilemap_p self = (tilemap_p)malloc(sizeof(tilemap_type));
    *self = (tilemap_type){
        .name = strdup(name),
            .image = image,
            .width = width,
            .height = height,
            .tilewidth = tilewidth,
            .tileheight = tileheight,
            .tilesets = NULL,
            .layers = NULL,
    };

    return self;
}

void add_layer(layer_p *head, layer_p layer)
{
    HASH_ADD_KEYPTR(hh, *head, layer->name, strlen(layer->name), layer);
}

void add_tileset(tileset_p *head, tileset_p tileset)
{
    HASH_ADD_KEYPTR(hh, *head, tileset->name, strlen(tileset->name), tileset);
}

tile_p tilemap_get_tile(tilemap_p self, unsigned gid)
{
    tile_p tile_tg = NULL;
    for (tileset_p tileset = self->tilesets;
            tileset != NULL;
            tileset = tileset->hh.next) {
        HASH_FIND_INT(self->tilesets->tiles, &gid, tile_tg);
        if (!tile_tg) {
            continue;
        } else {
            return tile_tg;
        }
    }

    return tile_tg;
}

void blit_tile(tilemap_p self,
            unsigned gid,
            SDL_Rect dest)
{
    if (gid == 0) return;
    tile_p tile_tg = NULL;
    tile_tg = tilemap_get_tile(self, gid);
    if (!tile_tg) {
        fprintf(stderr, "[ERROR] could not find tile gid: %d\n", gid);
        return;
    }
    tileset_p tileset_tg = self->tilesets;

    SDL_BlitSurface(tileset_tg->image,
            &tile_tg->source,
            self->image,
            &dest);
}

layer_p find_layer(layer_p *head, const char* layer_name)
{
    layer_p self = NULL;
    HASH_FIND_STR(*head, layer_name, self);
    if (!self) {
        fprintf(stderr, "[ERROR] Couldn't find layer: %s\n", layer_name);
    } else {
        fprintf(stderr, "[INFO] Found layer %s\n", self->name);
    }

    return self;
}

void blit_map(tilemap_p self, const char* layer_name)
{
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = self->tilewidth;
    dest.h = self->tileheight;

    layer_p cur_layer = NULL;
    cur_layer = find_layer(&(self->layers), layer_name);
    if (!cur_layer->cells) {
        fprintf(stderr, "[ERROR] layer %s is empty\n", cur_layer->name);
        return;
    }
    int *gid = (int *)utarray_front(cur_layer->cells);

    for (unsigned y = 0; y < self->height; y++) {
        for (unsigned x = 0; x < self->width; x++) {
            dest.x = x * self->tilewidth;
            dest.y = y * self->tileheight;
            blit_tile(self, *gid, dest);
            gid = (int *)utarray_next(cur_layer->cells, gid);
        }
    }

    return;
}

tilemap_p load_tmx_json(const char* filename)
{
    /* Create a return variable self, as well as the
     * JSON root and error pointers. The other variables are
     * declared upon their use. */
    tilemap_p self;
    json_t *root;
    json_error_t error;

    root = json_load_file(filename, 0, &error);
    if (!root) {
        fprintf(stderr,
                "[ERROR] file: %s: line: %d: %s\n",
                filename, error.line, error.text);
        return NULL;
    }

    /* root should be an object containing the map data
     * and properties */
    if (!json_is_object(root)) {
        fprintf(stderr, "[ERROR] root is not an object.\n");
        return NULL;
    }

    /* first get the width and height (in tiles) of the map
     * and the width and height of each tile. Create the tilemap
     * from this initial data */
    json_t *height, *width;
    json_t *tileheight, *tilewidth;

    height = json_object_get(root, "height");
    width = json_object_get(root, "width");
    tileheight = json_object_get(root, "tileheight");
    tilewidth = json_object_get(root, "tilewidth");

    self = tilemap("json_tilemap",
            json_integer_value(width),
            json_integer_value(height),
            json_integer_value(tilewidth),
            json_integer_value(tileheight),
            0xff030303);

    /* tilesets are stored as an array of objects. Get the array
     * first, the iterate and add to the tilemap */


    return self;
}
