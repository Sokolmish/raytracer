#ifndef __SCENE_H__
#define __SCENE_H__

#include "kdtree.hpp"

struct Camera {
    Vec3f pos, dir, up, right;
    int width, height;
    float depth, ratio, fov;
    Camera();
    Camera(int width, int height, const Vec3f &pos, const Vec3f &dir, const Vec3f &up, float fov);
};

class Scene {
private:
    std::vector<VolumeObj*> objects;
    std::vector<Light> lights;
    AABBbox globalBox;
    KDnode root;
public:
    void addObject(VolumeObj *obj);
    void addObject(const std::vector<VolumeObj*> &objs);
    void addLight(const Light &obj);
    const KDnode& getKDTree() const;
    const std::vector<Light>& getLights() const;
    void buildKDtree(int max_depth, int leaf, int parts);
    void free();
};


#endif