#ifndef __KDTREE_H__
#define __KDTREE_H__

#include "objects.hpp"

class KDnode {
private:
    AABBbox selfBox;
    KDnode *l, *r;
    int plane;
    float coord;
    std::vector<VolumeObj*> objs;
public:
    KDnode(const AABBbox &self);
    void setChilds(KDnode &left, KDnode &right);
    bool hasL() const;
    bool hasR() const;
    const KDnode& getL() const;
    const KDnode& getR() const;
    bool isIntersect(const Vec3f &origin, const Vec3f &dir) const;
    float getIntersection(const Vec3f &origin, const Vec3f &dir, VolumeObj **out) const;
};

#endif 