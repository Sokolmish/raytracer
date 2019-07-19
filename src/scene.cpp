#include "scene.hpp"

// SCENE

void Scene::addObject(VolumeObj *obj) {
    if (objects.size() == 0)
        globalBox = obj->boundingBox();
    else
        globalBox = globalBox.expand(obj->boundingBox());
    objects.push_back(obj);
}

void Scene::addObject(const std::vector<VolumeObj*> &objs) {
    for (auto&& t : objs)
        addObject(t);
}

void Scene::addLight(const Light &obj) {
    lights.push_back(obj);
}

const KDnode& Scene::getKDTree() const {
    return root;
}

const std::vector<Light>& Scene::getLights() const {
    return lights;
}

void Scene::buildKDtree(int max_depth, int leaf, int parts) { //leaf - maximum objects in leaf
    root = KDnode(NULL, globalBox, objects, max_depth, leaf, parts);
}

void Scene::free() {
    for (auto&& t : objects)
        delete t;
    root.free();
}

Color Environment::getColor(const Vec3f &origin, const Vec3f &dir) {
    return background;
}