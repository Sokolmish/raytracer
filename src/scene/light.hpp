#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "../util/util.hpp"

struct Light {
    Vec3f loc;
    float power;
    Light(Vec3f loc, float power) : loc(loc), power(power) {}
};

#endif