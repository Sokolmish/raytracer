#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "../util/util.hpp"

struct Light {
    Vec3f loc;
    float power;
    Color color;
    Light(const Vec3f &loc, float power, const Color &c) : loc(loc), power(power), color(c) {}
};

#endif