#ifndef __FLAT_TRIANGLE_H__
#define __FLAT_TRIANGLE_H__

#include "../objectsSupport.hpp"

class FlatTriangle : public VolumeObj {
private:
    Vec3f p1, p2, p3;
    Material mat;
    Vec3f norm;
    AABBbox bbox;
public:
    FlatTriangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Material &mat);
    Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const;
    AABBbox boundingBox() const;
};


#endif