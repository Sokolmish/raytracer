#include <iostream>

#include "image.h"
#include "vec.h"

#define FILENAME "./output.bmp"


int main(int argc, char **argv) {
    Image image(100, 100);
    
    for (int i = 0; i < 100; i++) {
        image.setPixel(i, i, Vec3i(255, 0, 0));
        image.setPixel(i, 99 - i, Vec3i(0, 0, 255));
    }

    image.write(FILENAME);
    return 0;
}