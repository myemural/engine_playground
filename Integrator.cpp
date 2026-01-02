//
// Created by oguzh on 20.12.2025.
//

#include "Integrator.h"

#include <iostream>

void Integrator::semi_implicit_euler(
    Body& b,
    const double dt
) {
    b.velocity += b.acceleration * dt;
    b.position += b.velocity * dt;
}
