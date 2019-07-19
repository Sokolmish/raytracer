#include "camera.hpp"

Camera::Camera() {
    this->width = 1280;
    this->height = 720;
    this->pos = Vec3f(0, 0, 0);
    this->dir = Vec3f(0, 0, -1);
    this->up = Vec3f(0, 1, 0);
    this->fov = toRad(90);
    this->depth = this->width / (2 * tan(this->fov / 2));
    this->right = this->dir ^ this->up;
}

Camera::Camera(int w, int h, const Vec3f &pos, const Vec3f &dir, const Vec3f &up, float fov) {
    this->width = w;
    this->height = h;
    this->pos = pos;
    this->dir = dir;
    this->up = up;
    this->fov = fov;
    this->depth = this->width / (2 * tan(this->fov / 2));
    this->right = this->dir ^ this->up;
}