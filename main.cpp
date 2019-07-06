#include <iostream>

#include "image.h"
#include "util.h"
#include "vec.h"

#define FILENAME "./output.bmp"

#define BACKGROUND WHITE //Later background will be changed to skybox

Color traceRay(Vec3f origin, Vec3f dir) {
    return BACKGROUND;
}

int main(int argc, char **argv) {

    int width = 100;
    int height = 100;

    Vec3f cameraPos(0, 0, 0);
    Vec3f cameraDir(0, 0, -1);
    float fov = toRad(90);

    Vec3f cameraUp(0, 1, 0); //This vector defines the vertical direction on screen
    Vec3f cameraRight = cameraDir ^ cameraUp;

    float ratio = width / height;
    float depth = width / (2 * tan(fov / 2)); //Length from camera to screen

    Image image(width, height);
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            int xSh = -width / 2 + i;
            int ySh = -height / 2 + j;
            Vec3f curDir = (cameraDir * depth + cameraUp * ySh + cameraRight * xSh).normalize();
            image.setPixel(i, j, traceRay(cameraPos, curDir));
        }
    }

    image.write(FILENAME);
    return 0;
}