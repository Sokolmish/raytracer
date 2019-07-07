#include "objects.hpp"

Sphere::Sphere(Vec3f center, float radius, Color color) {
    this->center = center;
    this->radius = radius;
    this->mat = color;
}

float Sphere::intersect(Vec3f origin, Vec3f dir) const {
    Vec3f shC = origin - center;
    float B = (shC * dir); // *2
    float C = (shC * shC) - radius * radius;
    float D = B * B - C; // -4C

    if (D < 0)
        return -1.f;
    else if (D - 0 < __FLT_EPSILON__)
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

Color Sphere::color(Vec3f touch) const {
    return mat;
}