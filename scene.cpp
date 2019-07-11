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

void Scene::buildKDtree(int max_depth, int leaf) { //leaf - maximum objects in leaf
    root = KDnode(globalBox);
    
    auto& box = root.selfBox;
    float xside = box.B.x - box.A.x;
    float yside = box.B.y - box.A.y;
    float zside = box.B.z - box.A.z;
    if (xside > std::max(yside, zside)) {
        float mid = (box.B.x + box.A.x) / 2.f;
        KDnode *l = new KDnode(AABBbox(Vec3f(box.A.x, box.A.y, box.A.z), Vec3f(mid, box.B.y, box.B.z)));
        KDnode *r = new KDnode(AABBbox(Vec3f(mid, box.A.y, box.A.z), Vec3f(box.B.x, box.B.y, box.B.z)));
        root.setChilds(PLANE_YZ, mid, l, r);
    }
    else if (yside > std::max(xside, zside)) {
        float mid = (box.B.y + box.A.y) / 2.f;
        KDnode *l = new KDnode(AABBbox(Vec3f(box.A.x, box.A.y, box.A.z), Vec3f(box.B.x, mid, box.B.z)));
        KDnode *r = new KDnode(AABBbox(Vec3f(box.A.x, mid, box.A.z), Vec3f(box.B.x, box.B.y, box.B.z)));
        root.setChilds(PLANE_XZ, mid, l, r);
    }
    else { //zside > std::max(xside, yside)
        float mid = (box.B.z + box.A.z) / 2.f;
        KDnode *l = new KDnode(AABBbox(Vec3f(box.A.x, box.A.y, box.A.z), Vec3f(box.B.x, box.B.y, mid)));
        KDnode *r = new KDnode(AABBbox(Vec3f(box.A.x, box.A.y, mid), Vec3f(box.B.x, box.B.y, box.B.z)));
        root.setChilds(PLANE_XY, mid, l, r);
    }

    for (auto&& t : objects) {
        // root.objs.push_back(t);
        if (root.l->selfBox.intersect(t->boundingBox()))
            root.l->objs.push_back(t);
        if (root.r->selfBox.intersect(t->boundingBox()))
            root.r->objs.push_back(t);
    }
}

void Scene::free() {
    for (auto&& t : objects)
        delete t;
    root.free();
}