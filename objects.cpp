#include "objects.hpp"
#include <iostream>

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

// SPHERE

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

// TRIANGLE

Triangle::Triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Material &mat) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->mat = mat;
    this->norm = ((p2 - p1) ^ (p3 - p1)).normalize();
}

float Triangle::intersect(const Vec3f &origin, const Vec3f &dir) const {
    //Moller and Trumbore algorithm
    Vec3f e1 = p2 - p1;
    Vec3f e2 = p3 - p1;
    Vec3f pvec = dir ^ e2;
    float det = e1 * pvec;
    if (fabs(det) < 1e-3) //1e-8?
        return -1.f;
    det = 1 / det;
    Vec3f tvec = origin - p1;
    float u = (tvec * pvec) * det;
    if (u < 0 || u > 1)
        return -1.f;
    Vec3f qvec = tvec ^ e1;
    float v = (dir * qvec) * det;
    if (v < 0 || u + v > 1)
        return -1.f;
    return (qvec * e2) * det;
}

Vec3f Triangle::normal(const Vec3f &touch) const {
    return this->norm;
}

Material Triangle::material(const Vec3f &touch) const {
    return mat;
}

//Complex objects

std::vector<Triangle*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat) {
    std::vector<Triangle*> t;
    t.push_back(new Triangle(p1, p2, p3, mat));
    t.push_back(new Triangle(p1, p3, p4, mat));
    return t;
}

std::vector<Triangle*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    std::vector<Triangle*> t;
    float med_3 = edge / (2 * sqrtf(3)); //Length of median divided by 3
    Vec3f p1(top.x, top.y - height, top.z - med_3 * 2);
    Vec3f p2(top.x - edge / 2, top.y - height, top.z + med_3);
    Vec3f p3(top.x + edge / 2, top.y - height, top.z + med_3);
    auto rot = MAT_ROT_Z(angle);
    p1 = (rot * (p1 - top)) + top;
    p2 = (rot * (p2 - top)) + top;
    p3 = (rot * (p3 - top)) + top;
    t.push_back(new Triangle(p1, p2, p3, mat));
    t.push_back(new Triangle(p1, p2, top, mat));
    t.push_back(new Triangle(p2, p3, top, mat));
    t.push_back(new Triangle(p3, p1, top, mat));
    return t;
}