#include "systems.h"
#include "components.h"

int render_entities(entity_p entity) {
    if (entity->components[TEXTURE] != NULL &&
            entity->components[POSITION] != NULL) {

        render_texture(entity->components[TEXTURE],
                get_position(entity->components[POSITION]));
    }

    if (entity->next != NULL) {
        return render_entities(entity->next);
    }

    return 0;
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

        self_pos->position.x += self_mot->vel_x;
        self_pos->position.y += self_mot->vel_y;

        clear_motion(entity);
    }

    if (entity->next != NULL) {
        return walk_entities(entity->next);
    }

    return 0;
}
