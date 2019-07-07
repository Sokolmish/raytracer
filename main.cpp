#include <iostream>
#include <vector>

#include "image.h"
#include "util.h"
#include "vec.h"
#include "objects.hpp"

#define FILENAME "./output.bmp"

#define BACKGROUND LIGHT_BLUE //Later background will be changed to skybox

Color traceRay(Vec3f origin, Vec3f dir, std::vector<Sphere> objects) {
    for (auto&& obj : objects) {
        float t = obj.intersect(origin, dir);
        if (t >= 0) {
            Vec3f touch = origin + t * dir;
            return obj.color(touch);
        }
        else {
            return BACKGROUND;
        }
    }
}

int main(int argc, char **argv) {
    std::vector<Sphere> objects;

    objects.push_back(Sphere(Vec3f(0, 0, -10), 5, RED));

    int width = 500; //Resolution
    int height = 250;

    Vec3f cameraPos(0, 0, 0);
    Vec3f cameraDir(0, 0, -1); //Direction of view
    Vec3f cameraUp(0, 1, 0); //This vector defines the vertical direction on screen

    float fov = toRad(120); //Generaly defines depth

    float ratio = width / height;
    float depth = width / (2 * tan(fov / 2)); //Length from camera to screen
    Vec3f cameraRight = cameraDir ^ cameraUp;

    Image image(width, height);
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int xSh = -width / 2 + i;
            int ySh = -height / 2 + j;
            Vec3f curDir = (cameraDir * depth + cameraUp * ySh + cameraRight * xSh).normalize();
            image.setPixel(i, j, traceRay(cameraPos, curDir, objects));
        }
    }

    image.write(FILENAME);
    return 0;
}