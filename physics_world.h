//
// Created by oguzh on 20.12.2025.
//

#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H
#include <cstdint>
#include <iosfwd>
#include <vector>

#include "physics_state.h"
#include "body.h"
#include "contact_manifold.h"

class PhysicsWorld {
public:
    explicit PhysicsWorld(double fixed_dt_seconds);

    void update(double frame_dt_seconds);

    void fixed_step(double dt);

    std::uint64_t step_count() const noexcept;

    void step_bodies_with_ccd(double dt, std::vector<ContactManifold> &manifolds);

    Vec2 position() const;

    Vec2 velocity() const;

    double accumulator() const;

    void step_bodies(double dt);

    const std::vector<ContactManifold>& getManifolds() const;

    std::vector<Body> bodies;

private:
    std::vector<ContactManifold> manifolds;
    const double m_fixed_dt;
    double m_accumulator = 0.0;
    std::uint64_t m_steps = 0;
};



#endif //PHYSICS_WORLD_H
