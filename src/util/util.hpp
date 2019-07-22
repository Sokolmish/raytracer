#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include <omp.h>

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
    Color ambient_color;
    Color diffuse_color;
    Color specular_color;
    float specular_exponent;
    float refractive_index;
    Vec2f albedo; //Reflection and refraction coefficients
    Material() = default;
    Material(const Color &acol, const Color &dcol, const Color &scol, const float Ni, const float spec, const Vec2f &a) : 
        ambient_color(acol), diffuse_color(dcol), specular_color(scol),
        specular_exponent(spec), refractive_index(Ni), albedo(a) {}
};

#define WHITE       Color(1.0, 1.0, 1.0)
#define BLACK       Color(0, 0, 0)
#define RED         Color(1.0, 0, 0)
#define GREEN       Color(0, 1.0, 0)
#define BLUE        Color(0, 0, 1.0)
#define YELLOW      Color(1.0, 1.0, 0)
#define MAGENTA     Color(1.0, 0, 1.0)
#define CYAN        Color(0, 1.0, 1.0)
#define GRAY        Color(0.5, 0.5, 0.5)
#define LIGHT_GRAY  Color(0.75, 0.75, 0.75)
#define LIGHT_BLUE  Color(0.67, 0.84, 0.9)

// #define IVORY           Material(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3),  50.f)
// #define GLASS           Material(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8),  125.f)

#define RED_RUBBER      Material(Vec3f(0, 0, 0), Vec3f(0.3, 0.1, 0.1), Vec3f(0.1, 0.1, 0.1), 1.0, 10.f, Vec2f(0, 0))
#define GREEN_RUBBER    Material(Vec3f(0, 0, 0), Vec3f(0.1, 0.3, 0.1), Vec3f(0.1, 0.1, 0.1), 1.0, 10.f, Vec2f(0, 0))
#define BLUE_RUBBER     Material(Vec3f(0, 0, 0), Vec3f(0.1, 0.1, 0.3), Vec3f(0.1, 0.1, 0.1), 1.0, 10.f, Vec2f(0, 0))
#define CYAN_RUBBER     Material(Vec3f(0, 0, 0), Vec3f(0.1, 0.3, 0.3), Vec3f(0.1, 0.1, 0.1), 1.0, 10.f, Vec2f(0, 0))
#define YELLOW_RUBBER   Material(Vec3f(0, 0, 0), Vec3f(0.3, 0.3, 0.1), Vec3f(0.1, 0.1, 0.1), 1.0, 10.f, Vec2f(0, 0))
#define MIRROR          Material(Vec3f(0, 0, 0), Vec3f(0.0, 0.0, 0.0), Vec3f(10.0, 10.0, 10.0), 1.0, 1000.f, Vec2f(0.8, 0))

#define MAT_IDENTITY Mat3f { 1, 0, 0, 0, 1, 0, 0, 0, 1 }
#define MAT_ROT_X(phi) Mat3f { 1, 0, 0, 0, cosf(phi), -sinf(phi), 0, sinf(phi), cosf(phi) }
#define MAT_ROT_Y(phi) Mat3f { cosf(phi), 0, sinf(phi), 0, 1, 0, -sinf(phi), 0, cosf(phi) }
#define MAT_ROT_Z(phi) Mat3f { cosf(phi), -sinf(phi), 0, sinf(phi), cosf(phi), 0, 0, 0, 1 }
#define MAT_SCALE(x, y, z) Mat3f { x, 0, 0, 0, y, 0, 0, 0, z }

#endif