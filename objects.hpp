#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "objectsSupport.hpp"

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

class Triangle : public VolumeObj {
private:
    Vec3f p1, p2, p3;
    Material mat;
    Vec3f norm;
    AABBbox bbox;
public:
    Triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Material &mat);
    Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const;
    AABBbox boundingBox() const;
};

class nTriangle : public VolumeObj { //For easier migration from Vec3f vertices to Vertex vertices
private:
    Vertex p1, p2, p3;
    Material mat;
    AABBbox bbox;
public:
    nTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat);
    Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const;
    AABBbox boundingBox() const;
};

std::vector<VolumeObj*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat);

std::vector<VolumeObj*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat); //Right triangular pyramid
std::vector<VolumeObj*> createSerpinsky(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat); //Serpinsky pyramid

#endif