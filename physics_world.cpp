//
// Created by oguzh on 20.12.2025.
//
// Simulation for fixed timestep
#include "physics_world.h"

#include <complex>
#include <iostream>

#include "Integrator.h"

PhysicsWorld::PhysicsWorld(double fixed_dt_seconds)
    : m_fixed_dt(fixed_dt_seconds) {}

/*
void PhysicsWorld::update(const double frame_dt_seconds) {
    m_accumulator += frame_dt_seconds;

    while (m_accumulator >= m_fixed_dt) {
        step();
        m_accumulator -= m_fixed_dt;
    }
}
*/

void PhysicsWorld::update(const double frame_dt_seconds) {
    m_accumulator += frame_dt_seconds;

    while (m_accumulator >= m_fixed_dt) {
        step(m_fixed_dt);
        m_accumulator -= m_fixed_dt;
    }
}

double PhysicsWorld::position() const {
    return m_state.position;
}

double PhysicsWorld::velocity() const {
    return m_state.velocity;
}

void PhysicsWorld::step() {
    Integrator::semi_implicit_euler(m_state, m_fixed_dt);
    ++m_steps;
}

void PhysicsWorld::step(double dt) {
    m_prev = m_curr;

    const double x0 = m_curr.position;
    const double v0 = m_curr.velocity;

    bool hit = false;
    double t_hit = dt;

    // 1) TOI (linear CCD)
    if (v0 > 0.0 && x0 < wall_x) {
        const double t = (wall_x - x0) / v0;
        if (t >= 0.0 && t <= dt) {
            hit = true;
            t_hit = t;
        }
    }

    // 2) integrate until hit (or full dt)
    m_curr.position = x0 + v0 * t_hit;


    // 3) resolve collision
    if (hit) {
        m_curr.velocity = -m_curr.velocity;

        std::cout << "[CCD] hit t=" << t_hit
                  << " x=" << m_curr.position
                  << " v=" << m_curr.velocity << "\n";
    }

    // 4) integrate remaining part with new velocity
    const double remaining = dt - t_hit;
    if (remaining > 0.0) {
        m_curr.position += m_curr.velocity * remaining;
    }

    ++m_steps;
    m_curr.velocity += m_curr.acceleration * dt;
}

const PhysicsState& PhysicsWorld::current() const { return m_curr; }
const PhysicsState& PhysicsWorld::previous() const { return m_prev; }


std::uint64_t PhysicsWorld::step_count() const noexcept {
    return m_steps;
}
