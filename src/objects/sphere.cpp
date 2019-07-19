#include "sphere.hpp"

Sphere::Sphere(const Vec3f &center, float radius, const Material &mat) : 
        bbox(center - radius * Vec3f(1, 1, 1), center + radius * Vec3f(1, 1, 1)) {
    this->center = center;
    this->radius = radius;
    this->mat = mat;
}

AABBbox Sphere::boundingBox() const {
    return bbox;
}

Intersection Sphere::getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const {
    Vec3f L = center - origin;
    float tca = L * dir;
    float d2 = L * L - tca * tca;
    if (d2 > radius * radius) 
        return Intersection(false, 0.f);
    float thc = sqrtf(radius * radius - d2);
    float t0 = tca - thc;
    if (t0 < 0) 
        t0 = tca + thc;
    if (t0 < 0) 
        return Intersection(false, 0.f);
    if (needData) {
        Vec3f touch = origin + dir * t0;
        return Intersection(true, t0, touch, (touch - center).normalize(), mat);
    }
    else
        return Intersection(true, t0);
}