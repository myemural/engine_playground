//
// Created by oguzh on 29.12.2025.
//

#ifndef RENDER_CONSOLE_H
#define RENDER_CONSOLE_H
#include <vector>

#include "contact_manifold.h"
#include "vec2.h"

struct ScreenPoint {
    int x;
    int y;
};

double interpolate(double prev, double curr, double alpha);
void render_console(double x, double wall_x);
Vec2 interpolate(const Vec2& prev,
                 const Vec2& curr,
                 double alpha);
void render_console_2d(const Vec2& pos,
                       double wall_x,
                       int width,
                       int height,
                       const std::vector<ContactManifold>& manifolds);
inline ScreenPoint world_to_screen(
    const Vec2& p,
    double wall_x,
    int width,
    int height
);

void draw_contact_point(
    std::vector<std::string>& grid,
    const Vec2& p,
    double wall_x
);

void draw_contact_normal(
    std::vector<std::string>& grid,
    const Vec2& p,
    const Vec2& n,
    double wall_x
);

void render_smoke_test();

#endif //RENDER_CONSOLE_H
