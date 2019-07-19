#include "objects.hpp"

std::vector<VolumeObj*> createQuadrangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Material &mat) {
    std::vector<VolumeObj*> t;
    Vec3f norm = (p2 - p1) ^ (p3 - p1);
    t.push_back(new Triangle(Vertex(p1, norm), Vertex(p2, norm), Vertex(p3, norm), mat));
    t.push_back(new Triangle(Vertex(p1, norm), Vertex(p3, norm), Vertex(p4, norm), mat));
    return t;
}

std::vector<VolumeObj*> createPyramid(const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    std::vector<VolumeObj*> t;
    float med_3 = edge / (2 * sqrtf(3)); //Length of median divided by 3
    Vec3f p1(top.x, top.y - height, top.z - med_3 * 2);
    Vec3f p2(top.x - edge / 2, top.y - height, top.z + med_3);
    Vec3f p3(top.x + edge / 2, top.y - height, top.z + med_3);
    auto rot = MAT_ROT_Y(angle);
    p1 = (rot * (p1 - top)) + top;
    p2 = (rot * (p2 - top)) + top;
    p3 = (rot * (p3 - top)) + top;
    t.push_back(new FlatTriangle(p3, p2, p1, mat));
    t.push_back(new FlatTriangle(p1, p2, top, mat));
    t.push_back(new FlatTriangle(p2, p3, top, mat));
    t.push_back(new FlatTriangle(p3, p1, top, mat));
    return t;
}

void addTriangles(std::vector<VolumeObj*> &dest, const std::vector<VolumeObj*> &src) {
    for (auto&& e : src)
        dest.push_back(e);
}

std::vector<VolumeObj*> createSerpinsky(int depth, const Vec3f &top, float height, float edge, float angle, const Material &mat) {
    std::vector<VolumeObj*> t;
    float med_3 = edge / (2 * sqrtf(3)); //Length of median divided by 3
    Vec3f p1(top.x, top.y - height, top.z - med_3 * 2);
    Vec3f p2(top.x - edge / 2, top.y - height, top.z + med_3);
    Vec3f p3(top.x + edge / 2, top.y - height, top.z + med_3);
    auto rot = MAT_ROT_Y(angle);
    p1 = (rot * (p1 - top)) + top;
    p2 = (rot * (p2 - top)) + top;
    p3 = (rot * (p3 - top)) + top;
    if (depth == 0) {
        t.push_back(new FlatTriangle(p3, p2, p1, mat));
        t.push_back(new FlatTriangle(p1, p2, top, mat));
        t.push_back(new FlatTriangle(p2, p3, top, mat));
        t.push_back(new FlatTriangle(p3, p1, top, mat));
    }
    else {
        addTriangles(t, createSerpinsky(depth - 1, top, height / 2, edge / 2, angle, mat));
        addTriangles(t, createSerpinsky(depth - 1, (top + p1) / 2.f, height / 2, edge / 2, angle, mat));
        addTriangles(t, createSerpinsky(depth - 1, (top + p2) / 2.f, height / 2, edge / 2, angle, mat));
        addTriangles(t, createSerpinsky(depth - 1, (top + p3) / 2.f, height / 2, edge / 2, angle, mat));
    }
    return t;
}