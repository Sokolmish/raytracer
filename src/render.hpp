#ifndef __RENDER_H__
#define __RENDER_H__

#include "image.hpp"
#include "scene.hpp"

#define ALLIASING_EDGE 80

class Render {
private:
    Environment env;
    int max_ray_depth;

    Intersection getIntersection(const Vec3f &origin, const Vec3f &dir, const Scene &scene, bool needData);
    Color traceRay(const Vec3f &origin, const Vec3f &dir, const Scene &scene, int depth);
public:
    Render(const Environment &env, int m) : env(env), max_ray_depth(m) {};
    void render(Image &image, const Camera &camera, const Scene &scene);
    void antiAlliasing(Image &image, const Camera &camera, const Scene &scene);
};

#endif