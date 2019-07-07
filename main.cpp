#include <iostream>
#include <vector>

#include "image.hpp"
#include "util.hpp"
#include "vec.hpp"
#include "objects.hpp"

#define FILENAME "./output.bmp"

#define BACKGROUND LIGHT_BLUE //Later background will be changed to skybox

Color traceRay(Vec3f origin, Vec3f dir, std::vector<Sphere> objects, std::vector<Light> lights) {
    Sphere *_target = NULL; 
    float tLen = __FLT_MAX__;
    for (auto&& obj : objects) {
        float t = obj.intersect(origin, dir);
        if (t >= 0 && t < tLen) { // If ray intersect an object
            _target = &obj;
            tLen = t;
        }
    }
    if (_target == NULL)
        return BACKGROUND;

    Sphere &target = *_target;
    Vec3f touch = origin + tLen * dir;
    Color diffCol = target.color(touch);
    
    float lightIntense = 0.f;
    for (auto&& light : lights) {
        Vec3f lDir = (light.loc - touch).normalize();
        float tPower = lDir * target.normal(touch);
        if (tPower > 0)
            lightIntense += tPower * light.power;
    }
    
    return diffCol * lightIntense;
}

int main(int argc, char **argv) {
    std::vector<Sphere> objects;
    std::vector<Light> lights;

    objects.push_back(Sphere(Vec3f(0, 0, -14), 3, RED));
    objects.push_back(Sphere(Vec3f(3, 4.5, -12.5), 2.5, BLUE));

    lights.push_back(Light(Vec3f(10, 25, 1), 1.2));

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