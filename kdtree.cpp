#include "kdtree.hpp"

KDnode::KDnode(KDnode *par, const AABBbox &self, const std::vector<VolumeObj*> &objs, int depth, int leaf_c, int parts) {
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
        Vec3f lbound, rbound;
        float step;
        Vec3f stepDir;
        if (xside > std::max(yside, zside)) {
            this->plane = PLANE_YZ;
            step = xside / parts;
            stepDir = Vec3f(1, 0, 0);
        }
        else if (yside > std::max(xside, zside)) {
            this->plane = PLANE_XZ;
            step = yside / parts;
            stepDir = Vec3f(0, 1, 0);
        }
        else { //zside > std::max(xside, yside)
            this->plane = PLANE_XY;
            step = zside / parts;
            stepDir = Vec3f(0, 0, 1);
        }
        std::vector<VolumeObj*> lObjs, rObjs;
        int opti = 1;
        float SAH = __FLT_MAX__;
        for (int i = 1; i < parts; i++) {
            std::vector<VolumeObj*> tlo, tro;
            AABBbox lbox = AABBbox(selfBox.A, selfBox.B - (parts - i) * step * stepDir);
            AABBbox rbox = AABBbox(selfBox.A + i * step * stepDir, selfBox.B);
            for (auto&& e : objects) {
                if (lbox.intersect(e->boundingBox()))
                    tlo.push_back(e);
                if (rbox.intersect(e->boundingBox()))
                    tro.push_back(e);
            }
            float tSAH = i * tlo.size() + (parts - i) * tro.size();
            if (SAH > tSAH) {
                SAH = tSAH;
                lObjs = tlo;
                rObjs = tro;
                opti = i;
            }
        }
        if (plane == PLANE_YZ)
            coord = (selfBox.A.x + opti * step);
        else if (plane == PLANE_XZ)
            coord = (selfBox.A.y + opti * step);
        else //if (plane = PLANE_XY)
            coord = (selfBox.A.z + opti * step);

        this->l = new KDnode(this, AABBbox(selfBox.A, selfBox.B - (parts - opti) * step * stepDir), lObjs, depth - 1, leaf_c, parts);
        this->r = new KDnode(this, AABBbox(selfBox.A + opti * step * stepDir, selfBox.B), rObjs, depth - 1, leaf_c, parts);
        objects.clear();
    }
}

bool KDnode::isIntersect(const Vec3f &origin, const Vec3f &dir) const {
    return selfBox.intersect(origin, dir);
}

Intersection KDnode::getIntersection(const Vec3f &origin, const Vec3f &dir, bool needData) const {
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

            Intersection t = first->getIntersection(origin, dir, needData);
            float touchComp = origComp + t.len * dirComp;
            if (left == (touchComp > coord)) {
                //if the touch point placed in other node (this is possible if the object contains in both nodes)
                Intersection t1 = second->getIntersection(origin, dir, needData);
                if (t1.len < t.len)
                    return t1;
                else
                    return t;
            }
            if (t.state) //if intersection was found in the first node and the object fully placed in that node
                return t;
            else //if intersection did not found in the first node that it loaceted in the second node
                return second->getIntersection(origin, dir, needData);
        }
        else { // if (!(lin && rin))
            if (rin)
                return r->getIntersection(origin, dir, needData);
            else //if (lin)
                return l->getIntersection(origin, dir, needData);
        }
    }
    else { //leaf node
        Intersection tInter(false, __FLT_MAX__);
        for (auto&& e : this->objects) {
            Intersection t = e->getIntersection(origin, dir, needData);
            if (t.state && t.len < tInter.len)
                tInter = t;
        }
        
        if (tInter.state)
            return tInter;
        else 
            return Intersection(false, 0.f);

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