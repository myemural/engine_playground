//
// Created by oguzh on 29.12.2025.
//
#include <iostream>
#include <algorithm>
#include "render_console.h"
#include <thread>
#include <chrono>
#include <cmath>

#include "contact.h"

double interpolate(double prev, double curr, double alpha) {
    return prev + (curr - prev) * alpha;
}

Vec2 interpolate(const Vec2& prev,
                 const Vec2& curr,
                 double alpha)
{
    return Vec2{
        interpolate(prev.x, curr.x, alpha),
        interpolate(prev.y, curr.y, alpha)
    };
}


void render_console(double x, double wall_x) {
    constexpr int width = 60;
    int pos = static_cast<int>((x / wall_x) * width);
    pos = std::clamp(pos, 0, width - 1);

    std::string line(width, '-');
    line[pos] = 'o';
    line[width - 1] = '|';
    std::cout << "\33[2K\r"; // satırı temizle + başa dön
    std::cout << line << std::flush;
}

void render_console_2d(const Vec2& pos,
                       double wall_x,
                       int width,
                       int height,
                       const std::vector<ContactManifold>& manifolds)
{
    double world_y_max = wall_x * static_cast<double>(height) / width;

    double nx = pos.x / wall_x;
    double ny = pos.y / world_y_max;

    nx = std::clamp(nx, 0.0, 1.0);
    ny = std::clamp(ny, 0.0, 1.0);

    int sx = static_cast<int>(nx * (width  - 1) + 0.5);
    int sy = static_cast<int>(ny * (height - 1) + 0.5);

    std::vector<std::string> grid(height, std::string(width, ' '));

    for (int r = 0; r < height; ++r)
        grid[r][width - 1] = '|';

    int draw_y = height - 1 - sy;
    grid[draw_y][sx] = 'o';

    for (const auto& m : manifolds) {
        for (int i = 0; i < m.pointCount; ++i) {
            const ContactPoint& cp = m.points[i];

            draw_contact_point(grid, cp.position, wall_x);
            draw_contact_normal(grid, cp.position, cp.normal, wall_x);
        }
    }

    std::cout << "\33[2J\33[H";
    for (const auto& row : grid)
        std::cout << row << '\n';



    std::cout << std::flush;
}

inline ScreenPoint world_to_screen(
    const Vec2& p,
    double wall_x,
    int width,
    int height
) {
    double world_y_max = wall_x * static_cast<double>(height) / width;

    double nx = p.x / wall_x;
    double ny = p.y / world_y_max;

    nx = std::clamp(nx, 0.0, 1.0);
    ny = std::clamp(ny, 0.0, 1.0);

    int sx = static_cast<int>(nx * (width  - 1) + 0.5);
    int sy = static_cast<int>(ny * (height - 1) + 0.5);

    return {
        sx,
        height - 1 - sy
    };
}

void draw_contact_point(
    std::vector<std::string>& grid,
    const Vec2& p,
    double wall_x
) {
    int height = grid.size();
    int width  = grid[0].size();

    ScreenPoint sp = world_to_screen(p, wall_x, width, height);
    grid[sp.y][sp.x] = 'x';
}

void draw_contact_normal(
    std::vector<std::string>& grid,
    const Vec2& p,
    const Vec2& n,
    double wall_x
) {
    int height = grid.size();
    int width  = grid[0].size();

    ScreenPoint base = world_to_screen(p, wall_x, width, height);

    constexpr int len = 3;

    for (int i = 1; i <= len; ++i) {
        Vec2 q = {
            p.x + n.x * 0.1 * i,
            p.y + n.y * 0.1 * i
        };

        ScreenPoint sp = world_to_screen(q, wall_x, width, height);

        if (sp.x >= 0 && sp.x < width &&
            sp.y >= 0 && sp.y < height)
        {
            grid[sp.y][sp.x] = (n.x < 0 ? '<' :
                                n.x > 0 ? '>' :
                                n.y > 0 ? '^' : 'v');
        }
    }
}


void render_smoke_test()
{
    std::cout << "\33[2J\33[H";
    std::cout << "SMOKE_TEST_RENDER\n";
    std::cout << "X\n";
    std::cout << std::flush;
}

