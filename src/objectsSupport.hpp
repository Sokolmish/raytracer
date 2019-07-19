#ifndef __OBJECTSSUPPORT_H__
#define __OBJECTSSUPPORT_H__

#include "util/util.hpp"

struct Vertex {
    Vec3f loc, norm;
    Vec2f texture;
    Vertex(const Vec3f &loc, const Vec3f &n, const Vec2f &texture) : loc(loc), norm(n), texture(texture) {}
    Vertex(const Vec3f &loc, const Vec3f &norm): loc(loc), norm(norm), texture(Vec2f(0, 0)) {}
    Vertex(const Vec3f &loc) : loc(loc), norm(Vec3f(0, 0, 0)), texture(Vec2f(0, 0)) {}
    Vertex() = default;
};

struct AABBbox {
    Vec3f A, B;
    AABBbox() = default;
    AABBbox(const Vec3f &a, const Vec3f &b) : A(a), B(b) {}
    bool intersect(const Vec3f &origin, const Vec3f &dir) const;
    bool intersect(const AABBbox &box) const;
    AABBbox expand(const AABBbox &box) const;
};

struct Intersection {
    bool state;
    float len;
    Vec3f touch;
    Vec3f normal;
    Material mat;
    Intersection() = default;
    Intersection(bool state, float l, const Vec3f &t = Vec3f(), const Vec3f &norm = Vec3f(), const Material &m = Material()) :
        state(state), len(l), touch(t), normal(norm), mat(m) {}
};

class VolumeObj {
public:
    virtual ~VolumeObj() = default;
    virtual AABBbox boundingBox() const = 0;
    virtual Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const = 0;
};

#endif