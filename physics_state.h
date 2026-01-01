//
// Created by oguzh on 20.12.2025.
//

#ifndef PHYSICS_STATE_H
#define PHYSICS_STATE_H
#include "vec2.h"

struct PhysicsState {
    double acceleration = 9.7;
    double velocity = 0.0;
    double position = 0.0;

    Vec2 acceleration2d {9.7 , 0.0};
    Vec2 velocity2d;
    Vec2 position2d;
};

#endif //PHYSICS_STATE_H
