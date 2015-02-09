#include <SDL2/SDL_image.h>
#include <assert.h>
#include <string.h>
#include <jansson.h>
#include "tmx.h"

struct tile *tile(const unsigned gid, const SDL_Rect source)
{
    struct tile *self = (struct tile *)malloc(sizeof(struct tile));
    *self = (struct tile) {
        .gid = gid, .source = source
    };

    return self;
}

void add_tile(struct tile **head, struct tile *tile)
{
    HASH_ADD_INT(*head, gid, tile);
}

struct tileset *tileset(
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

    struct tileset *self = (struct tileset *)malloc(sizeof(struct tileset));
    struct tile *tiles = NULL;

    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++, i++) {
            // note: this for loop also increments i
            source_rect.x = x * tilewidth;
            source_rect.y = y * tileheight;
            add_tile(&tiles, tile(firstgid + i, source_rect));
        }
    }

    *self = (struct tileset) {
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

void tileset_offset(struct tileset **self, unsigned x, unsigned y)
{
    (**self).x_pos = x;
    (**self).y_pos = y;
}

struct layer *layer(
    const char* name,
    double opacity,
    bool visible
) {
    struct layer *self = (struct layer *)malloc(sizeof(struct layer));
    *self = (struct layer) {
        .name = strdup(name),
         .opacity = opacity,
          .visible = visible
    };
    utarray_new(self->cells, &ut_int_icd);

    return self;
}

void layer_add_cells(struct layer **self, int gid)
{
    utarray_push_back((*self)->cells, &gid);
}

unsigned calc_width(unsigned width_ct, unsigned tilewidth)
{
    return width_ct * tilewidth;
}

unsigned calc_height(unsigned height_ct, unsigned tileheight)
{
    return height_ct * tileheight;
}

struct tilemap *tilemap(
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

    struct tilemap *self = (struct tilemap *)malloc(sizeof(struct tilemap));
    *self = (struct tilemap) {
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

void add_layer(struct layer **head, struct layer *layer)
{
    HASH_ADD_KEYPTR(hh, *head, layer->name, strlen(layer->name), layer);
}

void add_tileset(struct tileset **head, struct tileset *tileset)
{
    HASH_ADD_KEYPTR(hh, *head, tileset->name, strlen(tileset->name), tileset);
}

struct tile *tilemap_get_tile(struct tilemap *self, unsigned gid)
{
    struct tile *tile_tg = NULL;
    for (struct tileset *tileset = self->tilesets;
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

void blit_tile(struct tilemap *self,
               unsigned gid,
               SDL_Rect dest)
{
    if (gid == 0) return;
    struct tile *tile_tg = NULL;
    tile_tg = tilemap_get_tile(self, gid);
    if (!tile_tg) {
        fprintf(stderr, "[ERROR] could not find tile gid: %d\n", gid);
        return;
    }
    struct tileset *tileset_tg = self->tilesets;

    SDL_BlitSurface(tileset_tg->image,
                    &tile_tg->source,
                    self->image,
                    &dest);
}

struct layer *find_layer(struct layer **head, const char *name)
{
    struct layer *self = NULL;
    HASH_FIND_STR(*head, name, self);
    if (!self) {
        fprintf(stderr, "[ERROR] Couldn't find layer: %s\n", name);
    } else {
        fprintf(stderr, "[INFO] Found layer %s\n", self->name);
    }

    return self;
}

void blit_map(struct tilemap *self, const char *layer_name)
{
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = self->tilewidth;
    dest.h = self->tileheight;

    struct layer *cur_layer = NULL;
    cur_layer = find_layer(&(self->layers), layer_name);
    if (!cur_layer) {
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

struct tilemap *load_tmx_json(const char *filename)
{
    /* Create a return variable self, as well as the
     * JSON root and error pointers. The other variables are
     * declared upon their use. */
    struct tilemap *self;
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
    json_t *tilesets;
    tilesets = json_object_get(root, "tilesets");
    size_t index;
    json_t *value;
    json_array_foreach(tilesets, index, value) {
        json_t *firstgid, *imagefile, *name;
        firstgid = json_object_get(value, "firstgid");
        imagefile = json_object_get(value, "image");
        name = json_object_get(value, "name");
        tilewidth = json_object_get(value, "tilewidth");
        tileheight = json_object_get(value, "tileheight");
        struct tileset *temp_tileset;
        SDL_Surface *temp_image = IMG_Load(json_string_value(imagefile));
        temp_tileset = tileset(
                           json_integer_value(firstgid),
                           json_string_value(name),
                           json_integer_value(tilewidth),
                           json_integer_value(tileheight),
                           temp_image);
        add_tileset(&(self->tilesets), temp_tileset);
    }

    json_t *layers;
    layers = json_object_get(root, "layers");
    value = NULL;
    index = 0;
    json_array_foreach(layers, index, value) {
        json_t *name, *opacity, *visible, *data;
        struct layer *temp_layer;
        name = json_object_get(value, "name");
        opacity = json_object_get(value, "opacity");
        visible = json_object_get(value, "visible");
        data = json_object_get(value, "data");
        temp_layer = layer(json_string_value(name), json_real_value(opacity), json_integer_value(visible));
        size_t tile_index;
        json_t *tile_gid;
        json_array_foreach(data, tile_index, tile_gid) {
            layer_add_cells(&temp_layer, json_integer_value(tile_gid));
        }
        add_layer(&(self->layers), temp_layer);
    }

    return self;
}
