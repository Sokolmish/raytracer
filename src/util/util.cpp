#include "util.hpp"

float toRad(float deg) {
    return deg * (M_PI / 180);
}

Vec3f getNearPoint(const Vec3f &point, const Vec3f &ray, const Vec3f &normal) {
    if ((ray * normal) > 0) 
        return point + 1e-3f * normal;
    else
        return point - 1e-3f * normal;
}

Vec3f getReflection(const Vec3f &dir, const Vec3f &normal) {
    return dir - (2.f * (dir * normal)) * normal;
}

Vec3f getRefraction(const Vec3f &dir, const Vec3f &normal, float coeff) { //
    float cosi = -std::max(-1.f, std::min(1.f, (dir * normal)));
    float etai = 1, etat = coeff;
    Vec3f n = normal;
    if (cosi < 0) {
        cosi = -cosi;
        std::swap(etai, etat); 
        n = -normal;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3f() : dir * eta + n * (eta * cosi - sqrtf(k));
}