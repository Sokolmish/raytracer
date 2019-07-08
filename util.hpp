#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include <algorithm>
#include "vec.hpp"

float toRad(float deg);

Vec3f getNearPoint(const Vec3f &point, const Vec3f &ray, const Vec3f &normal);
Vec3f getReflection(const Vec3f &dir, const Vec3f &normal);
Vec3f getRefraction(const Vec3f &dir, const Vec3f &normal, float coeff);

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


#define IVORY           Material(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(102, 102, 77 ),  50.f)
#define GLASS           Material(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(153, 179, 204),  125.f)
#define RED_RUBBER      Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(77,  26,  26),   10.f)
#define GREEN_RUBBER    Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(26,  77,  26),   10.f)
#define BLUE_RUBBER     Material(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(26,  26,  77),   10.f)
#define MIRROR          Material(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(255, 255, 255),  1425.f)

#endif