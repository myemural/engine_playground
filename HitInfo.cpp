//
// Created by oguzh on 28.12.2025.
//

#include "HitInfo.h"

HitInfo::HitInfo() : state(false), t(0.0) {};

bool HitInfo::hit() const {
    return state;
}

void HitInfo::hit(bool s) {
     state = s;
}

double HitInfo::time() const {
    return  t;
}

void HitInfo::time(double t_hit) {
    t = t_hit;
}
