#ifndef __SCENE_H__
#define __SCENE_H__

#include "kdtree.hpp"
#include "parser/objModel.hpp"
#include "scene/light.hpp"
#include "scene/camera.hpp"
#include "scene/environment.hpp"

class Scene {
private:
    std::vector<VolumeObj*> objects;
    std::vector<Light> lights;
    AABBbox globalBox;
    KDnode root;
public:
    void addObject(VolumeObj *obj);
    void addObject(const std::vector<VolumeObj*> &objs);

    void addSphere(const Vec3f &center, float r, const Material &mat);
    void addTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Material &mat);
    void addQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat);
    void addPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat);
    void addSerpinskiy(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat);
    void addObjModel(const Vec3f &center, const Mat3f &trans, const std::string &path);

    void addLight(const Light &obj);
    void addLight(const Vec3f &loc, float power, const Color &col);

    const KDnode& getKDTree() const;
    const std::vector<Light>& getLights() const;
    void buildKDtree(int max_depth, int leaf, int parts);
    void free();
};

#endif