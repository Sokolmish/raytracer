#include "kdtree.hpp"

KDnode::KDnode(const AABBbox &self) {
    selfBox = self;
    l = NULL;
    r = NULL;
    objs = std::vector<VolumeObj*>();
}

void KDnode::setChilds(KDnode &left, KDnode &right) {
    l = &left;
    r = &right;
}

bool KDnode::hasL() const {
    return l != NULL;
}
bool KDnode::hasR() const {
    return r != NULL;
}

const KDnode& KDnode::getL() const {
    return *l;
}
const KDnode& KDnode::getR() const {
    return *r;
}

bool KDnode::isIntersect(const Vec3f &origin, const Vec3f &dir) const {
    return selfBox.intersect(origin, dir);
}

float KDnode::getIntersection(const Vec3f &origin, const Vec3f &dir, VolumeObj **out) const {
    *out = NULL;
    float len = __FLT_MAX__;
    for (auto&& e : objs) {
        float t = e->intersect(origin, dir);
        if (t >= 0 && t < len) {
            *out = e;
            len = t;
        }
    }
    return len;
}