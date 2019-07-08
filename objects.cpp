#include "objects.hpp"
#include <iostream>

Sphere::Sphere(const Vec3f &center, float radius, const Material &mat) {
    this->center = center;
    this->radius = radius;
    this->mat = mat;
}

float Sphere::intersect(const Vec3f &origin, const Vec3f &dir) const {
    Vec3f shC = origin - center;
    float B = (shC * dir);
    float C = (shC * shC) - radius * radius;
    float D = B * B - C;

    if (D < 0)
        return -1.f;
    else if (D - 0 < 1e-3)
        return -B;
    else {
        float t1 = (-B + sqrtf(D));
        float t2 = (-B - sqrtf(D));
        // t1 higher than t2 (Wow!)
        return t2 < 0 ? t1 : t2;
    }
}

Vec3f Sphere::normal(const Vec3f &touch) const {
    return (touch - center).normalize();
}

Material Sphere::material(const Vec3f &touch) const {
    return mat;
}

Camera::Camera() {
    this->width = 1280;
    this->height = 720;
    this->pos = Vec3f(0, 0, 0);
    this->dir = Vec3f(0, 0, -1);
    this->up = Vec3f(0, 1, 0);
    this->fov = toRad(90);
    this->ratio = this->width / this->height;
    this->depth = this->width / (2 * tan(this->fov / 2));
    this->right = this->dir ^ this->up;
}

Camera::Camera(int width, int height, const Vec3f &pos, const Vec3f &dir, const Vec3f &up, float fov) {
    this->width = width;
    this->height = height;
    this->pos = pos;
    this->dir = dir;
    this->up = up;
    this->fov = fov;
    this->ratio = this->width / this->height;
    this->depth = this->width / (2 * tan(this->fov / 2));
    this->right = this->dir ^ this->up;
}