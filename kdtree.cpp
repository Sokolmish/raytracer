#include "kdtree.hpp"

KDnode::KDnode(KDnode *par, const AABBbox &self, const std::vector<VolumeObj*> &objs, int depth, int leaf_c) {
    //leaf_c - max number of objects in the leaf node
    this->par = par;
    l = NULL;
    r = NULL;
    selfBox = self;
    objects = objs;
    if (depth > 0 && objects.size() >= leaf_c) {
        float xside = selfBox.B.x - selfBox.A.x;
        float yside = selfBox.B.y - selfBox.A.y;
        float zside = selfBox.B.z - selfBox.A.z;
        AABBbox lbox, rbox;
        if (xside > std::max(yside, zside)) {
            this->plane = PLANE_YZ;
            this->coord = (selfBox.B.x + selfBox.A.x) / 2.f;
            lbox = AABBbox(Vec3f(selfBox.A.x, selfBox.A.y, selfBox.A.z), Vec3f(coord, selfBox.B.y, selfBox.B.z));
            rbox = AABBbox(Vec3f(coord, selfBox.A.y, selfBox.A.z), Vec3f(selfBox.B.x, selfBox.B.y, selfBox.B.z));
        }
        else if (yside > std::max(xside, zside)) {
            this->plane = PLANE_XZ;
            this->coord = (selfBox.B.y + selfBox.A.y) / 2.f;
            lbox = AABBbox(Vec3f(selfBox.A.x, selfBox.A.y, selfBox.A.z), Vec3f(selfBox.B.x, coord, selfBox.B.z));
            rbox = AABBbox(Vec3f(selfBox.A.x, coord, selfBox.A.z), Vec3f(selfBox.B.x, selfBox.B.y, selfBox.B.z));
        }
        else { //zside > std::max(xside, yside)
            this->plane = PLANE_XY;
            this->coord = (selfBox.B.z + selfBox.A.z) / 2.f;
            lbox = AABBbox(Vec3f(selfBox.A.x, selfBox.A.y, selfBox.A.z), Vec3f(selfBox.B.x, selfBox.B.y, coord));
            rbox = AABBbox(Vec3f(selfBox.A.x, selfBox.A.y, coord), Vec3f(selfBox.B.x, selfBox.B.y, selfBox.B.z));
        }
        std::vector<VolumeObj*> lObjs, rObjs;
        for (auto&& e : objects) {
            if (lbox.intersect(e->boundingBox()))
                lObjs.push_back(e);
            if (rbox.intersect(e->boundingBox()))
                rObjs.push_back(e);
        }
        this->l = new KDnode(this, lbox, lObjs, depth - 1, leaf_c);
        this->r = new KDnode(this, rbox, rObjs, depth - 1, leaf_c);
        objects.clear();
    }
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
            if (left == (touchComp > coord)) {
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
        for (auto&& e : this->objects) {
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