#include "image.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstring>

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    data = new uint8_t[width * height * 3];
    memset(data, 0, width * height * 3); 
}

void Image::free() {
    delete[] data;
}

void Image::setPixel(int x, int y, const Color &color) {
    float max = std::max(std::max(color[0], color[1]), color[2]);
    data[(width * y + x) * 3 + 0] = (uint8_t) (max > 255 ? color[0] / max * 255 : color[0]);
    data[(width * y + x) * 3 + 1] = (uint8_t) (max > 255 ? color[1] / max * 255 : color[1]);
    data[(width * y + x) * 3 + 2] = (uint8_t) (max > 255 ? color[2] / max * 255 : color[2]);
}

Color Image::getPixel(int x, int y) const {
    Color t;
    t[0] = data[(width * y + x) * 3 + 0];
    t[1] = data[(width * y + x) * 3 + 1];
    t[2] = data[(width * y + x) * 3 + 2];
    return t;
}

int Image::write(const char *path) const {
    stbi_flip_vertically_on_write(true);
    return stbi_write_bmp(path, width, height, 3, data);
}