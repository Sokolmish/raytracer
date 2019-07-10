#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "util.hpp"

struct AABBbox {
    Vec3f A, B;
    AABBbox() = default;
    AABBbox(const Vec3f &A, const Vec3f &b);
    bool intersect(const Vec3f &origin, const Vec3f &dir) const;
    AABBbox expand(const AABBbox &box) const;
};

class VolumeObj {
public:
    virtual float intersect(const Vec3f &origin, const Vec3f &dir) const = 0;
    virtual Vec3f normal(const Vec3f &touch) const = 0;
    virtual Material material(const Vec3f &touch) const = 0;

    virtual AABBbox boundingBox() const = 0;
};


class Sphere : public VolumeObj {
private:
    Vec3f center;
    float radius;
    Material mat;
    AABBbox bbox;
public:
    Sphere(const Vec3f &center, float radius, const Material &mat);

    float intersect(const Vec3f &origin, const Vec3f &dir) const;
    Vec3f normal(const Vec3f &touch) const;
    Material material(const Vec3f &touch) const;

    AABBbox boundingBox() const;
};

class Triangle : public VolumeObj {
private:
    Vec3f p1, p2, p3;
    Material mat;
    Vec3f norm;
    AABBbox bbox;
public:
    Triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Material &mat);

    float intersect(const Vec3f &origin, const Vec3f &dir) const;
    Vec3f normal(const Vec3f &touch) const;
    Material material(const Vec3f &touch) const;

    AABBbox boundingBox() const;
};

std::vector<VolumeObj*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat);

std::vector<VolumeObj*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat); //Right triangular pyramid
std::vector<VolumeObj*> createSerpinsky(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat); //Serpinsky pyramid
#endif