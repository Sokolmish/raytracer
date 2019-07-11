#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <initializer_list>

#include "vec.hpp"
#include "matrix.hpp"

float toRad(float deg);

Vec3f getNearPoint(const Vec3f &point, const Vec3f &ray, const Vec3f &normal);
Vec3f getReflection(const Vec3f &dir, const Vec3f &normal);
Vec3f getRefraction(const Vec3f &dir, const Vec3f &normal, float coeff);

#define RES_8K 7680, 4320
#define RES_4K 3840, 2160
#define RES_1080p 1920, 1080
#define RES_720p 1280, 720

typedef Vec3f Color;

struct Material {
    float refractive_index;
    Vec4f albedo;
    Color diffuse_color;
    float specular_exponent;
    Material() = default;
    Material(const float r, const Vec4f &a, const Color &color, const float spec) : 
        refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
};

#define WHITE       Color(255, 255, 255)
#define BLACK       Color(0, 0, 0)
#define RED         Color(255, 0, 0)
#define GREEN       Color(0, 255, 0)
#define BLUE        Color(0, 0, 255)
#define YELLOW      Color(255, 255, 0)
#define MAGENTA     Color(255, 0, 255)
#define CYAN        Color(0, 255, 255)
#define GRAY        Color(128, 128, 128)
#define LIGHT_GRAY  Color(192, 192, 192)
#define LIGHT_BLUE  Color(173, 216, 230)

#define IVORY           Material(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(102, 102, 77 ),  50.f)
#define GLASS           Material(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(153, 179, 204),  125.f)
#define MIRROR          Material(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(255, 255, 255),  1425.f)
#define RED_RUBBER      Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(77,  26,  26),   10.f)
#define GREEN_RUBBER    Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(26,  77,  26),   10.f)
#define BLUE_RUBBER     Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(26,  26,  77),   10.f)
#define CYAN_RUBBER     Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(26,  77,  77),   10.f)
#define YELLOW_RUBBER   Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(77,  77,  26),   10.f)

#define MAT_ROT_X(phi) Matrix<float, 3, 3> { 1, 0, 0, 0, cosf(phi), -sinf(phi), 0, sinf(phi), cosf(phi) }
#define MAT_ROT_Y(phi) Matrix<float, 3, 3> { cosf(phi), 0, sinf(phi), 0, 1, 0, -sinf(phi), 0, cosf(phi) }
#define MAT_ROT_Z(phi) Matrix<float, 3, 3> { cosf(phi), -sinf(phi), 0, sinf(phi), cosf(phi), 0, 0, 0, 1 }

#endif