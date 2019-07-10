#ifndef __KDTREE_H__
#define __KDTREE_H__

#include "objects.hpp"

#define PLANE_XY 1
#define PLANE_YZ 2
#define PLANE_XZ 3

class KDnode {
private:
    AABBbox selfBox;
    KDnode *l, *r;
    int plane;
    float coord;
    std::vector<VolumeObj*> objs;

    friend class Scene;
public:
    KDnode() = default;
    KDnode(const AABBbox &self);
    void setChilds(int plane, float coord, KDnode *left, KDnode *right);
    bool hasL() const;
    bool hasR() const;
    const KDnode& getL() const;
    const KDnode& getR() const;
    bool isIntersect(const Vec3f &origin, const Vec3f &dir) const;
    float getIntersection(const Vec3f &origin, const Vec3f &dir, VolumeObj **out) const;
    void free();
};

#endif 