#include "image.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "util/stb_image_write.h"

#include <cstring>

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    data = new Color[width * height];
    memset(data, 0, width * height * sizeof(Color)); 
}

void Image::free() {
    delete[] data;
}

void Image::setPixel(int x, int y, const Color &color) {
    data[width * y + x] = color;
}

Color Image::getPixel(int x, int y) const {
    return data[width * y + x];
}

int Image::write(const char *path) const {
    unsigned char *buf = new unsigned char[width * height * 3];
    #pragma omp parallel for
    for (int i = 0; i < width * height; i++) {
        Color color = data[i];
        float max = std::max(std::max(color[0], color[1]), color[2]);
        buf[i * 3 + 0] = (uint8_t) (max > 1.f ? color[0] / max * 255 : color[0] * 255);
        buf[i * 3 + 1] = (uint8_t) (max > 1.f ? color[1] / max * 255 : color[1] * 255);
        buf[i * 3 + 2] = (uint8_t) (max > 1.f ? color[2] / max * 255 : color[2] * 255);
    }
    stbi_flip_vertically_on_write(true);
    auto ret = stbi_write_bmp(path, width, height, 3, buf);
    delete[] buf;
    return ret;
}