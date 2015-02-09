#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "uthash.h"

struct entity {
    const char *name;
    struct component *components;
    void *data;
    UT_hash_handle hh;
};

struct entity 	*entity(const char *name, void **data);
void 		entity_add_component(
    struct entity **self, struct component *component);
void 		*entity_get_component(
    struct entity **self, const char *name);

#include "components.h"
/**** Special Entities ****/
extern const int STEP_DX;

typedef enum HEALTH {
    DEAD, DANGER, CAUTION, HURT, FINE
} Health;

typedef enum SANITY {
    GONE, PSYCHOTIC, DELUSIONAL, PARANOID, COGENT
} Sanity;

typedef enum ACTION {
    IDLE, ATTACK, INSPECT
} Action;

struct player {
    /***** Character Attributes *****
     ********************************/
    char *player_name;
    Health health;
    Sanity sanity;
    Action action;
    int hold_time, frame_time;
};

struct entity *player(
    const char *name,
    const char *imagefile,
    const int w, const int h,
    SDL_Point pos,
    int direction);
int player_update(struct entity **self);
void player_render(struct entity **self, SDL_Renderer *renderer);
void player_destroy(struct entity **self);

#endif
