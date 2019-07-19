#include "objectsSupport.hpp"

//AABB box

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