#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "actor.h"

extern const int STEP_DX;

typedef enum frame_e {
    STOPPED, IDLE, RIGHT_FOOT, LEFT_FOOT
} FRAME;

typedef enum health_e {
    DEAD, DANGER, CAUTION, HURT, FINE
} HEALTH;

typedef enum sanity_e {
    GONE, PSYCHOTIC, DELUSIONAL, PARANOID, COGENT
} SANITY;

typedef enum action_e {
    STOP, WALK, ATTACK, INSPECT
} ACTION;

typedef struct player_s player_type, *player_p;

struct player_s {
    /***** Character Attributes *****
     * image:
     * image_default:
     * clipping: clipping rectangle
     * location: location rectangle
     * orient:
     * hold_time:
     * walk_state:
     * dx:
     * frame:
     * set_sprite():
     ********************************/
    actor_type;
    HEALTH health;
    SANITY sanity;
    ACTION action;
    FRAME frame;
    int hold_time, frame_time;
    int dx;
    int (*update)(player_p self, int dt, _Bool *key);
    void (*render)(player_p self, SDL_Renderer *renderer);
};

player_p player_constructor(char filename[], int loc_x, int loc_y,
        ORIENTATION orientation, SDL_Renderer *renderer);

void player_destructor(player_p self);

#endif
