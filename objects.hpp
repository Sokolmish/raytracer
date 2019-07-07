#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "vec.h"
#include "util.h"

// class VolumeObject {
// public:
//     //Returns distance to touch (If it's negative, then there is no intersection)
//     virtual float intersect(Vec3f origin, Vec3f dir) const { throw "ABSTRACT"; }; 

//     virtual Vec3f normal(Vec3f touch) const { throw "ABSTRACT"; };
//     virtual Color color(Vec3f touch) const { throw "ABSTRACT"; };
// };

class Sphere /* : public VolumeObject */ {
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

#endif