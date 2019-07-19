#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "../util/util.hpp"

class Environment {
private:
    Color background;
public:
    Environment(Color b) : background(b) {}
    Color getColor(const Vec3f &origin, const Vec3f &dir);
};


#endif