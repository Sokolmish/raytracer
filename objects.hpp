#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "util.hpp"
#include <vector>
#include <initializer_list>

struct Camera {
    Vec3f pos, dir, up, right;
    int width, height;
    float depth, ratio, fov;
    Camera();
    Camera(int width, int height, const Vec3f &pos, const Vec3f &dir, const Vec3f &up, float fov);
};


struct Material {
    float refractive_index;
    Vec4f albedo;
    Color diffuse_color;
    float specular_exponent;
    Material(const float r, const Vec4f &a, const Color &color, const float spec) {
        refractive_index = r;
        albedo = a;
        diffuse_color = color;
        specular_exponent = spec;
    }
    Material() {
        refractive_index = 0;
        albedo = Vec4f();
        diffuse_color = Color();
        specular_exponent = 0;
    }
};

struct AABBbox {
    Vec3f A, B;
    AABBbox(const Vec3f &A, const Vec3f &b);
    bool intersect(const Vec3f &origin, const Vec3f &dir) const;
};


class VolumeObj {
public:
    virtual float intersect(const Vec3f &origin, const Vec3f &dir) const = 0;
    virtual Vec3f normal(const Vec3f &touch) const = 0;
    virtual Material material(const Vec3f &touch) const = 0;
};

struct Light {
    Vec3f loc;
    float power;
    Light(Vec3f loc, float power) {
        this->loc = loc;
        this->power = power;
    }
};

struct Scene {
    std::vector<VolumeObj*> objects;
    std::vector<Light> lights;
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
};

std::vector<Triangle*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat);

std::vector<Triangle*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat); //Right triangular pyramid
std::vector<Triangle*> createSerpinsky(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat); //Serpinsky pyramid
#endif