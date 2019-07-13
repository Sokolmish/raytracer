#include "objects.hpp"

//  AABB box

AABBbox::AABBbox(const Vec3f &A, const Vec3f &B) {
    this->A = A;
    this->B = B;
}

bool AABBbox::intersect(const Vec3f &origin, const Vec3f &dir) const {
    float lo = (A.x - origin.x) / dir.x;
    float hi = (B.x - origin.x) / dir.x;
    float tmin = std::min(lo, hi);
    float tmax = std::max(lo, hi);
    if (tmax < 0 || tmin > tmax)
        return false;

    lo = (A.y - origin.y) / dir.y;
    hi = (B.y - origin.y) / dir.y;
    tmin = std::max(tmin, std::min(lo, hi));
    tmax = std::min(tmax, std::max(lo, hi));
    if (tmax < 0 || tmin > tmax)
        return false;

    lo = (A.z - origin.z) / dir.z;
    hi = (B.z - origin.z) / dir.z;
    tmin = std::max(tmin, std::min(lo, hi));
    tmax = std::min(tmax, std::max(lo, hi));
    if (tmax < 0 || tmin > tmax)
        return false;
    return true;
}

AABBbox AABBbox::expand(const AABBbox &box) const {
    AABBbox t;
    t.A.x = std::min(A.x, box.A.x);
    t.A.y = std::min(A.y, box.A.y);
    t.A.z = std::min(A.z, box.A.z);
    t.B.x = std::max(B.x, box.B.x);
    t.B.y = std::max(B.y, box.B.y);
    t.B.z = std::max(B.z, box.B.z);
    return t;
}

bool AABBbox::intersect(const AABBbox &box) const {
    if (B.x < box.A.x || A.x > box.B.x) 
        return false;
    if (B.y < box.A.y || A.y > box.B.y) 
        return false;
    if (B.z < box.A.z || A.z > box.B.z) 
        return false;
    return true;
}

// SPHERE

Sphere::Sphere(const Vec3f &center, float radius, const Material &mat) : 
        bbox(center - radius * Vec3f(1, 1, 1), center + radius * Vec3f(1, 1, 1)) {
    this->center = center;
    this->radius = radius;
    this->mat = mat;
}

float Sphere::intersect(const Vec3f &origin, const Vec3f &dir) const {
    Vec3f L = center - origin;
    float tca = L * dir;
    float d2 = L * L - tca * tca;
    if (d2 > radius * radius) 
        return -1.f;
    float thc = sqrtf(radius * radius - d2);
    float t0 = tca - thc;
    if (t0 < 0) 
        t0 = tca + thc;
    if (t0 < 0) 
        return -1.f;
    return t0;
}

Vec3f Sphere::normal(const Vec3f &touch) const {
    return (touch - center).normalize();
}

Material Sphere::material(const Vec3f &touch) const {
    return mat;
}

AABBbox Sphere::boundingBox() const {
    return bbox;
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

float Triangle::intersect(const Vec3f &origin, const Vec3f &dir) const {
    if (!bbox.intersect(origin, dir))
        return -1.f;
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

AABBbox Triangle::boundingBox() const {
    return bbox;
}

//Complex objects

std::vector<VolumeObj*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat) {
    std::vector<VolumeObj*> t;
    t.push_back(new Triangle(p1, p2, p3, mat));
    t.push_back(new Triangle(p1, p3, p4, mat));
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
    t.push_back(new Triangle(p1, p2, p3, mat));
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
        t.push_back(new Triangle(p1, p2, p3, mat));
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

//Vertex and nTriangle

Vertex::Vertex(Vec3f loc, Vec3f norm, Vec2f texture) {
    this->loc = loc;
    this->norm = norm;
    this->texture = texture;
}

Vertex::Vertex(Vec3f loc, Vec3f norm) {
    this->loc = loc;
    this->norm = norm;
    this->texture = texture;
}

Vertex::Vertex(Vec3f loc) {
    this->loc = loc;
    this->norm = norm;
    this->texture = texture;
}

nTriangle::nTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->mat = mat;
    this->bbox = AABBbox(
        Vec3f(std::min({ p1.loc.x, p2.loc.x, p3.loc.x }), std::min({ p1.loc.y, p2.loc.y, p3.loc.y }), std::min({ p1.loc.z, p2.loc.z, p3.loc.z })),
        Vec3f(std::max({ p1.loc.x, p2.loc.x, p3.loc.x }), std::max({ p1.loc.y, p2.loc.y, p3.loc.y }), std::max({ p1.loc.z, p2.loc.z, p3.loc.z })));
}

float nTriangle::intersect(const Vec3f &origin, const Vec3f &dir) const {
    if (!bbox.intersect(origin, dir))
        return -1.f;
    //Moller and Trumbore algorithm
    Vec3f e1 = p2.loc - p1.loc;
    Vec3f e2 = p3.loc - p1.loc;
    Vec3f pvec = dir ^ e2;
    float det = e1 * pvec;
    if (fabs(det) < 1e-3) //1e-8?
        return -1.f;
    det = 1 / det;
    Vec3f tvec = origin - p1.loc;
    float u = (tvec * pvec) * det;
    if (u < 0 || u > 1)
        return -1.f;
    Vec3f qvec = tvec ^ e1;
    float v = (dir * qvec) * det;
    if (v < 0 || u + v > 1)
        return -1.f;
    return (qvec * e2) * det;
}

Vec3f nTriangle::normal(const Vec3f &touch) const {
    throw "NOT_IMPLEMENTED";
}

Material nTriangle::material(const Vec3f &touch) const {
    return mat;
}

AABBbox nTriangle::boundingBox() const {
    return bbox;
}