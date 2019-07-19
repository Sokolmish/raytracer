#include "flatTriangle.hpp"

FlatTriangle::FlatTriangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Material &mat) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->mat = mat;
    this->norm = ((p2 - p1) ^ (p3 - p1)).normalize();
    this->bbox = AABBbox(
        Vec3f(std::min({ p1.x, p2.x, p3.x }), std::min({ p1.y, p2.y, p3.y }), std::min({ p1.z, p2.z, p3.z })),
        Vec3f(std::max({ p1.x, p2.x, p3.x }), std::max({ p1.y, p2.y, p3.y }), std::max({ p1.z, p2.z, p3.z })));
}

AABBbox FlatTriangle::boundingBox() const {
    return bbox;
}

Intersection FlatTriangle::getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const {
    if (!bbox.intersect(origin, dir))
        return Intersection(false, 0.f);
    //Moller and Trumbore algorithm
    Vec3f e1 = p2 - p1;
    Vec3f e2 = p3 - p1;
    Vec3f pvec = dir ^ e2;
    float det = e1 * pvec;
    if (fabs(det) < 1e-3) //1e-8?
        return Intersection(false, 0.f);
    det = 1 / det;
    Vec3f tvec = origin - p1;
    float u = (tvec * pvec) * det;
    if (u < 0 || u > 1)
        return Intersection(false, 0.f);
    Vec3f qvec = tvec ^ e1;
    float v = (dir * qvec) * det;
    if (v < 0 || u + v > 1)
        return Intersection(false, 0.f);
    float t0 = (qvec * e2) * det;
    if (t0 < 0)
        return Intersection(false, 0.f);
    if (needData)
        return Intersection(true, t0, origin + t0 * dir, norm, mat);
    else
        return Intersection(true, t0);
}