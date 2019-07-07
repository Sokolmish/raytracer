#ifndef __UTIL_H__
#define __UTIL_H__

#include <cmath>
#include "vec.hpp"

typedef Vec3i Color;

#define WHITE Color(255, 255, 255)
#define BLACK Color(0, 0, 0)
#define RED Color(255, 0, 0)
#define GREEN Color(0, 255, 0)
#define BLUE Color(0, 0, 255)
#define YELLOW Color(255, 255, 0)
#define MAGENTA Color(255, 0, 255)
#define CYAN Color(0, 255, 255)
#define GRAY Color(128, 128, 128)
#define LIGHT_GRAY Color(192, 192, 192)
#define LIGHT_BLUE Color(173, 216, 230)

float toRad(float deg);

#endif