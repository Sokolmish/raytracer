#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "util/util.hpp"

struct Camera {
    Vec3f pos, dir, up, right;
    int width, height;
    float depth, fov;
    Camera();
    Camera(int width, int height, const Vec3f &pos, const Vec3f &dir, const Vec3f &up, float fov);
};

#endif