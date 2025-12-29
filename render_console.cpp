//
// Created by oguzh on 29.12.2025.
//
#include <iostream>
#include <algorithm>
#include "render_console.h"
#include <thread>
#include <chrono>


double interpolate(double prev, double curr, double alpha) {
    return prev + (curr - prev) * alpha;
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

