#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include "vec.hpp"

float toRad(float deg);

typedef Vec3f Color;

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

struct Material {
    float refractive_index;
    Vec4f albedo;
    Vec3f diffuse_color;
    float specular_exponent;
    Material(const float r, const Vec4f &a, const Vec3f &color, const float spec) {
        refractive_index = r;
        albedo = a;
        diffuse_color = color;
        specular_exponent = spec;
    }
};

#define IVORY       Material(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3),   50.)
#define GLASS       Material(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8),  125.)
#define RED_RUBBER  Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1),   10.)
#define MIRROR      Material(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.)

#endif