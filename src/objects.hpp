#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "objects/sphere.hpp"
#include "objects/flatTriangle.hpp"
#include "objects/triangle.hpp"

std::vector<VolumeObj*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat);

std::vector<VolumeObj*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat); //Right triangular pyramid
std::vector<VolumeObj*> createSerpinsky(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat); //Serpinsky pyramid

#endif