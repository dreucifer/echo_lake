#include <SDL2/SDL.h>
#include <string.h>
#include "game.h"
#include "systems.h"
#include "components.h"

int render_entities(struct entity **cur_entity)
{
    SDL_Point cam = world_get()->cam;
    struct position *cur_pos = position_from_entity(cur_entity);
    struct texture *cur_tex = texture_from_entity(cur_entity);
    SDL_Rect rect = {0, 0, 0, 0};

    if (!cur_tex) {
        return -1;
    }

    if (cur_pos) {
        rect = (SDL_Rect) {
            .x = cur_pos->pos.x - cam.x - (cur_pos->size.x * 0.5),
             .y = cur_pos->pos.y - cam.y - (cur_pos->size.y * 0.5),
              .w = 80,
               .h = 80
        };
        SDL_RenderCopy(
            game_get()->renderer,
            cur_tex->image,
            &(cur_tex->section),
            &rect);
    } else {
        rect = (SDL_Rect) {
            cam.x, cam.y, WIDTH, HEIGHT
        };
        SDL_RenderCopy(
            game_get()->renderer,
            cur_tex->image,
            &rect,
            NULL);
    }


    return ((*cur_entity)->hh.next != NULL) ? render_entities((struct entity **)&(*cur_entity)->hh.next) : 0;
}

int walk_entities(struct entity **cur_entity)
{
    SDL_Point bounds = world_get()->bounds;
    struct position *self_pos;
    struct motion *self_mot;

    if (entity_get_component(cur_entity, "position") != NULL &&
            entity_get_component(cur_entity, "motion") != NULL)  {

        self_pos = position_from_entity(cur_entity);
        self_mot = motion_from_entity(cur_entity);

        switch (position_get_dir(cur_entity)) {
        default:
            break;
        case UP:
            self_pos->pos.y += -self_mot->vel;
            break;
        case DOWN:
            self_pos->pos.y += self_mot->vel;
            break;
        case LEFT:
            self_pos->pos.x += -self_mot->vel;
            break;
        case RIGHT:
            self_pos->pos.x += self_mot->vel;
            break;
        }

        if (self_pos->pos.x - 40 < 0) {
            self_pos->pos.x = 40;
        }

        if (self_pos->pos.y - 40 < 0) {
            self_pos->pos.y = 40;
        }

        if (self_pos->pos.x + 40 > bounds.x) {
            self_pos->pos.x = bounds.x - 40;
        }

        if (self_pos->pos.y + 40 > bounds.y) {
            self_pos->pos.y = bounds.y - 40;
        }

        motion_clear(cur_entity);
    }

    if ((*cur_entity)->hh.next != NULL) {
        return walk_entities((struct entity **)&(*cur_entity)->hh.next);
    }

    return 0;
}

void camera_follow(struct entity **target)
{
    SDL_Point bounds;
    SDL_Point *cam = &(world_get()->cam);
    struct position *target_pos = position_from_entity(target);

    bounds.x = world_get()->bounds.x - WIDTH;
    bounds.y = world_get()->bounds.y - HEIGHT;

    cam->x = target_pos->pos.x - ( WIDTH / 2 );
    cam->y = target_pos->pos.y - ( HEIGHT / 2 );

    if (cam->x < 0) {
        cam->x = 0;
    }

    if (cam->y < 0) {
        cam->y = 0;
    }

    if (cam->x > bounds.x) {
        cam->x = bounds.x;
    }

    if (cam->y > bounds.y) {
        cam->y = bounds.y;
    }
}
