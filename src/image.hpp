#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "util/util.hpp"

class Image {
private:
    int width, height;
    uint8_t *data;
public:
    Image(int width, int height);
    void free();

    void setPixel(int x, int y, const Color &color);
    Color getPixel(int x, int y) const;

    int write(const char *path) const;
};

#endif