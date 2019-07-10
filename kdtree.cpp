#include "kdtree.hpp"

KDnode::KDnode(const AABBbox &self) {
    selfBox = self;
    l = NULL;
    r = NULL;
    objs = std::vector<VolumeObj*>();
}

void KDnode::setChilds(int plane, float coord, KDnode *left, KDnode *right) {
    this->plane = plane;
    this->coord = coord;
    l = left;
    r = right;
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
    if (l != NULL) { // && (r != NULL)
        bool lin = l->isIntersect(origin, dir);
        bool rin = r->isIntersect(origin, dir);
        if (lin && rin) {
            bool left;
            VolumeObj *o1, *o2;
            float t1 = l->getIntersection(origin, dir, &o1);
            float t2 = r->getIntersection(origin, dir, &o2);
            if (t1 <= t2) {
                *out = o1;
                return t1;
            }
            else {
                *out = o2;
                return t2;
            }
        }
        else {
            if (rin)
                return r->getIntersection(origin, dir, out);
            else //if (lin)
                return l->getIntersection(origin, dir, out);
        }
    }
    else { //leaf node
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
}

void KDnode::free() {
    if (l != NULL) {
        l->free();
        delete l;
    }
    if (r != NULL) {
        r->free();
        delete r;
    }
}