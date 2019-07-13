#ifndef __OBJECTSSUPPORT_H__
#define __OBJECTSSUPPORT_H__

#include "util.hpp"

struct Vertex {
    Vec3f loc, norm;
    Vec2f texture;
    Vertex(Vec3f loc, Vec3f n, Vec2f texture) : loc(loc), norm(n), texture(texture) {}
    Vertex(Vec3f loc, Vec3f norm): loc(loc), norm(norm), texture(Vec2f(0, 0)) {}
    Vertex(Vec3f loc) : loc(loc), norm(Vec3f(0, 0, 0)), texture(Vec2f(0, 0)) {}
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

class VolumeObj {
public:
    virtual ~VolumeObj() = default;
    virtual float intersect(const Vec3f &origin, const Vec3f &dir) const = 0;
    virtual Vec3f normal(const Vec3f &touch) const = 0;
    virtual Material material(const Vec3f &touch) const = 0;
    virtual AABBbox boundingBox() const = 0;
};

#endif