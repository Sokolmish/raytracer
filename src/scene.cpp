#include "scene.hpp"

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


void Scene::addSphere(const Vec3f &center, float r, const Material &mat) {
    addObject(new Sphere(center, r, mat));
}

void Scene::addTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat) {
    addObject(new Triangle(p1, p2, p3, mat));
}

void Scene::addQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat) {
    addObject(createQuadrangle(p1, p2, p3, p4, mat));
}

void Scene::addPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    addObject(createPyramid(top, height, edge, angle, mat));
}

void Scene::addSerpinskiy(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    addObject(createSerpinsky(depth, top, height, edge, angle, mat));
}

void Scene::addObjModel(const Vec3f &center, const Mat3f &trans, const std::string &path) {
    addObject(ObjModel(center, trans, path).getObjects());
}


void Scene::addLight(const Light &obj) {
    lights.push_back(obj);
}

void Scene::addLight(const Vec3f &loc, float power, const Color &col) {
    addLight(Light(loc, power, col));
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