#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstring>

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    data = new unsigned  char[width * height * 3];
    memset(data, 0, width * height * 3); 
}

Image::~Image() {
    delete[] data;
}

void Image::setPixel(int x, int y, Vec3i color) {
    data[(width * y + x) * 3 + 0] = (unsigned char) color[0];
    data[(width * y + x) * 3 + 1] = (unsigned char) color[1];
    data[(width * y + x) * 3 + 2] = (unsigned char) color[2];
}

Vec3i Image::getPixel(int x, int y) const {
    Vec3i t;
    t[0] = data[(width * y + x) * 3 + 0];
    t[1] = data[(width * y + x) * 3 + 1];
    t[2] = data[(width * y + x) * 3 + 2];
    return t;
}

int Image::write(const char *path) const {
    stbi_flip_vertically_on_write(true);
    return stbi_write_bmp(path, width, height, 3, data);
}