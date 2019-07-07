#include "objects.hpp"
#include <iostream>

Sphere::Sphere(Vec3f center, float radius, Material mat) {
    this->center = center;
    this->radius = radius;
    this->mat = mat;
}

float Sphere::intersect(Vec3f origin, Vec3f dir) const {
    Vec3f shC = origin - center;
    float B = (shC * dir); // *2
    float C = (shC * shC) - radius * radius;
    float D = B * B - C; // -4C

    if (D < 0)
        return -1.f;
    else if (D - 0 < 1e-3)
        return -B; // /2.f
    else {
        float t1 = (-B + sqrtf(D)); // /2.f
        float t2 = (-B - sqrtf(D)); // /2.f
        // t1 higher than t2 (Wow!)
        return t2 < 0 ? t1 : t2;
    }
}

Vec3f Sphere::normal(Vec3f touch) const {
    return (touch - center).normalize();
}

Material Sphere::material(Vec3f touch) const {
    return mat;
}