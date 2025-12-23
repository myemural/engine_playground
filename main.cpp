#include <iostream>
#include <thread>
#include "engine_time.h"
#include "physics_world.h"
#include <random>

#include "physics_world_variable.h"

std::mt19937 rng{std::random_device{}()};
std::uniform_int_distribution<int> jitter_ms(-5, 5);

int main() {
    constexpr double physics_dt = 1.0 / 60.0; // 60 Hz physics
    PhysicsWorld world(physics_dt);
    physics_world_variable world_variable(physics_dt);

    //collision scenario
    double wall_x = 10;
    double x_old = 0.0;

    auto last = engine::now();

    for (int frame = 0; frame < 300; ++frame) {
        auto now = engine::now();
        std::chrono::duration<double> frame_dt = now - last;
        last = now;
        int base_ms = 16; // ~60 FPS
        int jitter = jitter_ms(rng); // simulation of unsteady frame pacing
        x_old = world.position() ? world.position() : 0.0;

        world.update(frame_dt.count());
        world_variable.update(frame_dt.count());

        if (x_old < wall_x && world.position() >= wall_x) {
            std::cout << "Collision occured" << '\n';
        }

        std::cout << "--------Fixed timestep--------" << '\n';
        std::cout << "Frame: " << frame
                  << " | Physics steps: "
                  << world.step_count() << '\n';

        std::cout << frame << ", " << world.velocity() << ", " << world.position() << "\n";

        std::cout << "--------Varying Timestep--------" << '\n';
        std::cout << "Frame: " << frame
                  << " | Physics steps: "
                  << world_variable.step_count() << '\n';
        std::cout << frame << ", " << world_variable.velocity() << ", " << world_variable.position() << "\n";


        std::this_thread::sleep_for(std::chrono::milliseconds(base_ms + jitter));
    }
}
