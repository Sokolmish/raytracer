#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include "vec.hpp"

class Image {
private:
    int width, height;
    unsigned char *data;

    //Disabling copying ability
    Image(const Image&) = delete;
    void operator=(const Image&) = delete;
public:
    Image(int width, int height);
    ~Image();

    void setPixel(int x, int y, Vec3i color);
    Vec3i getPixel(int x, int y) const;

    int write(const char *path) const;
};

#endif