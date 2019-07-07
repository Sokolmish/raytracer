#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "vec.hpp"
#include "util.hpp"

class Sphere {
private:
    Vec3f center;
    float radius;
    Color mat;
public:
    Sphere(Vec3f center, float radius, Color color);

    float intersect(Vec3f origin, Vec3f dir) const;
    Vec3f normal(Vec3f touch) const;
    Color color(Vec3f touch) const;
};

struct Light {
    Vec3f loc;
    float power;
    Light(Vec3f loc, float power) {
        this->loc = loc;
        this->power = power;
    }
};


#endif