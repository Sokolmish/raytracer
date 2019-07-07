#include <iostream>
#include <vector>

#include "image.hpp"
#include "util.hpp"
#include "vec.hpp"
#include "objects.hpp"

#define FILENAME "./output.bmp"

#define BACKGROUND LIGHT_BLUE //Later background will be changed to skybox

Color traceRay(Vec3f origin, Vec3f dir, std::vector<Sphere> objects, std::vector<Light> lights) {
    bool intersected = false;
    Color tCol = BACKGROUND;
    float tLen = __FLT_MAX__;
    float lightIntense = 0.f;
    for (auto&& obj : objects) {
        float t = obj.intersect(origin, dir);
        if (t >= 0) { // If ray intersect an object
            if (t < tLen) {
                intersected = true;

                Vec3f touch = origin + t * dir;
                tLen = t;
                tCol = obj.color(touch);

                for (auto&& light : lights) {
                    Vec3f lDir = (light.loc - touch).normalize();
                    float tPower = lDir * obj.normal(touch);
                    if (tPower > 0)
                        lightIntense += tPower * light.power;
                }
            }
        }
    }
    if (intersected)
        return tCol * lightIntense;
    else
        return BACKGROUND;
}

int main(int argc, char **argv) {
    std::vector<Sphere> objects;
    std::vector<Light> lights;

    objects.push_back(Sphere(Vec3f(0, 0, -14), 3, RED));
    objects.push_back(Sphere(Vec3f(2, 4, -14.5), 1.5, BLUE));

    lights.push_back(Light(Vec3f(10, 25, -12), 1));
    lights.push_back(Light(Vec3f(-9, 5, -10), 1.2));

    int width = 1024; //Resolution
    int height = 768;

    Vec3f cameraPos(0, 0, 0);
    Vec3f cameraDir(0, 0, -1); //Direction of view
    Vec3f cameraUp(0, 1, 0); //This vector defines the vertical direction on screen

    float fov = toRad(90); //Generaly defines depth 

    float ratio = width / height;
    float depth = width / (2 * tan(fov / 2)); //Length from camera to screen
    Vec3f cameraRight = cameraDir ^ cameraUp;

    Image image(width, height);
    
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int xSh = -width / 2 + i;
            int ySh = -height / 2 + j;
            Vec3f curDir = (cameraDir * depth + cameraUp * ySh + cameraRight * xSh).normalize();
            image.setPixel(i, j, traceRay(cameraPos, curDir, objects, lights));
        }
    }

    image.write(FILENAME);
    return 0;
}