#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "../objectsSupport.hpp"

class Triangle : public VolumeObj {
private:
    Vertex p1, p2, p3;
    Material mat;
    AABBbox bbox;
public:
    Triangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat);
    Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const;
    AABBbox boundingBox() const;
};


#endif