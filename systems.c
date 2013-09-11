#include <SDL2/SDL.h>
#include "game.h"
#include "systems.h"
#include "components.h"

int render_entities(entity_p entity) {
    component_p position = entity->components[POSITION];
    component_p texture = entity->components[TEXTURE];

    if (texture != NULL && position != NULL) {
        SDL_RenderCopy(
                game_singleton()->renderer,
                get_texture(entity),
                get_section(entity),
                get_position(entity));

    } else if (texture != NULL && position == NULL) {
        SDL_RenderCopy(
                game_singleton()->renderer,
                get_texture(entity),
                get_section(entity),
                NULL);
    }

    return (entity->next != NULL) ? render_entities(entity->next) : 0;
}

int walk_entities(entity_p entity) {
    position_p self_pos;
    motion_p self_mot;

    if (entity->components[POSITION] != NULL &&
            entity->components[MOTION] != NULL)  {

        self_pos =
            entity->components[POSITION]->delegate;
        self_mot =
            entity->components[MOTION]->delegate;

        switch (get_direction(entity)) {
            default:
                break;
            case UP:
                fprintf(stderr, "UP\n");
                self_pos->position.y += -self_mot->vel;
                break;
            case DOWN:
                self_pos->position.y += self_mot->vel;
                break;
            case LEFT:
                self_pos->position.x += -self_mot->vel;
                break;
            case RIGHT:
                self_pos->position.x += self_mot->vel;
                break;
        }

        clear_motion(entity);
    }

    if (entity->next != NULL) {
        return walk_entities(entity->next);
    }

    return 0;
}
