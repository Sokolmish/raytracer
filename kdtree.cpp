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
            float origComp, dirComp; //Component of vector, that orthogonal to separating plane
            if (plane == PLANE_XY) {
                origComp = origin.z;
                dirComp = dir.z;
            }
            else if (plane == PLANE_XZ) {
                origComp = origin.y;
                dirComp = dir.y;
            }
            else {
                origComp = origin.x;
                dirComp = dir.x;
            }
            
            KDnode *first, *second; //first and second nodes on the way of the ray
            bool left;
            if (origComp < coord) {
                left = true;
                first = l;
                second = r;
            }
            else {
                left = false;
                first = r;
                second = l;
            }

            float t = first->getIntersection(origin, dir, out);
            float touchComp = origComp + t * dirComp;
            if (left == (touchComp > coord)) { //(left && touchComp  > coord) || (!left && touchComp <= coord)
                //if the touch point placed in other node (this is possible if the object contains in both nodes)
                VolumeObj *o;
                float t1 = second->getIntersection(origin, dir, &o);
                if (t1 < t) {
                    *out = o;
                    return t1;
                }
                else
                    return t;
            }
            if (*out != NULL) //if intersection was found in the first node and the object fully placed in that node
                return t;
            else //if intersection did not found in the first node that it loaceted in the second node
                return second->getIntersection(origin, dir, out);
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