#ifndef __KDTREE_H__
#define __KDTREE_H__

#include "objects.hpp"

#define PLANE_XY 1
#define PLANE_YZ 2
#define PLANE_XZ 3

class KDnode {
private:
    AABBbox selfBox;
    KDnode *l, *r, *par;
    int plane;
    float coord;
    std::vector<VolumeObj*> objects;
    int max_depth, nodes_in_leaf;
    friend class Scene;
public:
    KDnode() = default;
    KDnode(KDnode *par, const AABBbox &self, const std::vector<VolumeObj*> &objs, int depth, int leaf_c, int parts);
    bool isIntersect(const Vec3f &origin, const Vec3f &dir) const;
    float getIntersection(const Vec3f &origin, const Vec3f &dir, VolumeObj **out) const;
    void free();
};

#endif 