#ifndef __SCENE_H__
#define __SCENE_H__

#include "kdtree.hpp"
#include "light.hpp"
#include "camera.hpp"

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

class Environment {
private:
    Color background;
public:
    Environment(Color b) : background(b) {}
    Color getColor(const Vec3f &origin, const Vec3f &dir);
};

#endif