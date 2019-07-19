#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "../objectsSupport.hpp"

class Sphere : public VolumeObj {
private:
    Vec3f center;
    float radius;
    Material mat;
    AABBbox bbox;
public:
    Sphere(const Vec3f &center, float radius, const Material &mat);
    Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const;
    AABBbox boundingBox() const;
};

#endif