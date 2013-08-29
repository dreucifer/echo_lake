#ifndef MOTION_H
#define MOTION_H

enum DIRECTION {
    UP, DOWN, LEFT, RIGHT
};

struct motions_s {
    int total_displacement;
    int target_displacement;
    int velocity;
    enum DIRECTION direction;
};

#endif
