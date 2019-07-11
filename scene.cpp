#include "scene.hpp"

// CAMERA

Camera::Camera() {
    this->width = 1280;
    this->height = 720;
    this->pos = Vec3f(0, 0, 0);
    this->dir = Vec3f(0, 0, -1);
    this->up = Vec3f(0, 1, 0);
    this->fov = toRad(90);
    this->ratio = this->width / this->height;
    this->depth = this->width / (2 * tan(this->fov / 2));
    this->right = this->dir ^ this->up;
}

Camera::Camera(int width, int height, const Vec3f &pos, const Vec3f &dir, const Vec3f &up, float fov) {
    this->width = width;
    this->height = height;
    this->pos = pos;
    this->dir = dir;
    this->up = up;
    this->fov = fov;
    this->ratio = this->width / this->height;
    this->depth = this->width / (2 * tan(this->fov / 2));
    this->right = this->dir ^ this->up;
}

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