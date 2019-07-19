#include "triangle.hpp"

Triangle::Triangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->mat = mat;
    this->bbox = AABBbox(
        Vec3f(std::min({ p1.loc.x, p2.loc.x, p3.loc.x }), std::min({ p1.loc.y, p2.loc.y, p3.loc.y }), std::min({ p1.loc.z, p2.loc.z, p3.loc.z })),
        Vec3f(std::max({ p1.loc.x, p2.loc.x, p3.loc.x }), std::max({ p1.loc.y, p2.loc.y, p3.loc.y }), std::max({ p1.loc.z, p2.loc.z, p3.loc.z })));
}

Intersection Triangle::getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const {
    if (!bbox.intersect(origin, dir))
        return Intersection(false, 0.f);
    //Moller and Trumbore algorithm
    Vec3f e1 = p2.loc - p1.loc;
    Vec3f e2 = p3.loc - p1.loc;
    Vec3f pvec = dir ^ e2;
    float det = e1 * pvec;
    if (fabs(det) < 1e-3) //1e-8?
        return Intersection(false, 0.f);
    det = 1 / det;
    Vec3f tvec = origin - p1.loc;
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
    if (needData) {
        Vec3f norm = ((1 - u - v) * p1.norm + u * p2.norm + v * p3.norm).normalize();
        //TODO: texture interpolation
        return Intersection(true, t0, origin + t0 * dir, norm, mat);
    }
    else
        return Intersection(true, t0);
}

AABBbox Triangle::boundingBox() const {
    return bbox;
}