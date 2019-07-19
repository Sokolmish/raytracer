#include "objects.hpp"

// SPHERE

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

// TRIANGLE

Triangle::Triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Material &mat) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->mat = mat;
    this->norm = ((p2 - p1) ^ (p3 - p1)).normalize();
    this->bbox = AABBbox(
        Vec3f(std::min({ p1.x, p2.x, p3.x }), std::min({ p1.y, p2.y, p3.y }), std::min({ p1.z, p2.z, p3.z })),
        Vec3f(std::max({ p1.x, p2.x, p3.x }), std::max({ p1.y, p2.y, p3.y }), std::max({ p1.z, p2.z, p3.z })));
}

AABBbox Triangle::boundingBox() const {
    return bbox;
}

Intersection Triangle::getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const {
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

//nTriangle

nTriangle::nTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->mat = mat;
    this->bbox = AABBbox(
        Vec3f(std::min({ p1.loc.x, p2.loc.x, p3.loc.x }), std::min({ p1.loc.y, p2.loc.y, p3.loc.y }), std::min({ p1.loc.z, p2.loc.z, p3.loc.z })),
        Vec3f(std::max({ p1.loc.x, p2.loc.x, p3.loc.x }), std::max({ p1.loc.y, p2.loc.y, p3.loc.y }), std::max({ p1.loc.z, p2.loc.z, p3.loc.z })));
}

Intersection nTriangle::getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const {
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

AABBbox nTriangle::boundingBox() const {
    return bbox;
}


//Complex objects

std::vector<VolumeObj*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat) {
    std::vector<VolumeObj*> t;
    Vec3f norm = (p2 - p1) ^ (p3 - p1);
    t.push_back(new nTriangle(Vertex(p1, norm), Vertex(p2, norm), Vertex(p3, norm), mat));
    t.push_back(new nTriangle(Vertex(p1, norm), Vertex(p3, norm), Vertex(p4, norm), mat));
    return t;
}

std::vector<VolumeObj*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    std::vector<VolumeObj*> t;
    float med_3 = edge / (2 * sqrtf(3)); //Length of median divided by 3
    Vec3f p1(top.x, top.y - height, top.z - med_3 * 2);
    Vec3f p2(top.x - edge / 2, top.y - height, top.z + med_3);
    Vec3f p3(top.x + edge / 2, top.y - height, top.z + med_3);
    auto rot = MAT_ROT_Y(angle);
    p1 = (rot * (p1 - top)) + top;
    p2 = (rot * (p2 - top)) + top;
    p3 = (rot * (p3 - top)) + top;
    // t.push_back(new Triangle(p1, p2, p3, mat));
    t.push_back(new Triangle(p1, p2, top, mat));
    t.push_back(new Triangle(p2, p3, top, mat));
    t.push_back(new Triangle(p3, p1, top, mat));
    return t;
}

void addTriangles(std::vector<VolumeObj*> &dest, const std::vector<VolumeObj*> &src) {
    for (auto&& e : src)
        dest.push_back(e);
}

std::vector<VolumeObj*> createSerpinsky(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    std::vector<VolumeObj*> t;
    float med_3 = edge / (2 * sqrtf(3)); //Length of median divided by 3
    Vec3f p1(top.x, top.y - height, top.z - med_3 * 2);
    Vec3f p2(top.x - edge / 2, top.y - height, top.z + med_3);
    Vec3f p3(top.x + edge / 2, top.y - height, top.z + med_3);
    auto rot = MAT_ROT_Y(angle);
    p1 = (rot * (p1 - top)) + top;
    p2 = (rot * (p2 - top)) + top;
    p3 = (rot * (p3 - top)) + top;
    if (depth == 0) {
        t.push_back(new Triangle(p3, p2, p1, mat));
        t.push_back(new Triangle(p1, p2, top, mat));
        t.push_back(new Triangle(p2, p3, top, mat));
        t.push_back(new Triangle(p3, p1, top, mat));
    }
    else {
        addTriangles(t, createSerpinsky(depth - 1, top, height / 2, edge / 2, angle, mat));
        addTriangles(t, createSerpinsky(depth - 1, (top + p1) / 2.f, height / 2, edge / 2, angle, mat));
        addTriangles(t, createSerpinsky(depth - 1, (top + p2) / 2.f, height / 2, edge / 2, angle, mat));
        addTriangles(t, createSerpinsky(depth - 1, (top + p3) / 2.f, height / 2, edge / 2, angle, mat));
    }
    return t;
}