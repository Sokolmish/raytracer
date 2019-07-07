#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "vec.hpp"
#include "util.hpp"

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

class VolumeObj {
public:
    virtual float intersect(Vec3f origin, Vec3f dir) const = 0;
    virtual Vec3f normal(Vec3f touch) const = 0;
    virtual Material material(Vec3f touch) const = 0;
};

class Sphere : public VolumeObj {
private:
    Vec3f center;
    float radius;
    Material mat;
public:
    Sphere(Vec3f center, float radius, Material mat);

    float intersect(Vec3f origin, Vec3f dir) const;
    Vec3f normal(Vec3f touch) const;
    Material material(Vec3f touch) const;
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

struct Camera {
    Vec3f pos, dir, up, right;
    float width, height, depth, ratio, fov;
};

#endif