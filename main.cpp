#include <iostream>
#include <vector>

#include "image.hpp"
#include "util.hpp"
#include "vec.hpp"
#include "objects.hpp"

#define FILENAME "./output.bmp"

#define BACKGROUND LIGHT_BLUE //Later background will be changed to skybox
#define MAX_DEPTH 4

float getIntersection(Vec3f origin, Vec3f dir, std::vector<Sphere> objects, Sphere **out) {
    *out = NULL;
    float len = __FLT_MAX__;
    for (auto&& obj : objects) {
        float t = obj.intersect(origin, dir);
        if (t >= 0 && t < len) {
            *out = &obj;
            len = t;
        }
    }
    return len;
}

Color traceRay(Vec3f origin, Vec3f dir, std::vector<Sphere> objects, std::vector<Light> lights, int depth) {
    if (depth >= MAX_DEPTH)
        return BACKGROUND;

    Sphere *_target;
    float tLen = getIntersection(origin, dir, objects, &_target);

    if (_target == NULL)
        return BACKGROUND;

    Sphere &target = *_target;
    Vec3f touch = origin + tLen * dir;
    Vec3f normal = target.normal(touch);
    Material mat = target.material(touch);

    Color reflColor = BLACK, refrColor = BLACK; //Black don't affect to color in addition
    if (mat.albedo[2] >= 1e-3) {
        Vec3f reflDir = getReflection(dir, normal);
        Vec3f reflTouch = getNearPoint(touch, reflDir, normal);
        reflColor = traceRay(reflTouch, reflDir, objects, lights, depth + 1);
    }
    if (mat.albedo[3] >= 1e-3) {
        Vec3f refrDir = getRefraction(dir, normal, mat.refractive_index);
        Vec3f refrTouch = getNearPoint(touch, refrDir, normal);
        refrColor = traceRay(refrTouch, refrDir, objects, lights, depth + 1);
    }
    
    float diffIntense = 0.f;
    float specIntense = 0.f;
    for (auto&& light : lights) {
        Vec3f lDir = (light.loc - touch).normalize(); //Light direction
        Vec3f lightTouch = getNearPoint(touch, lDir, normal);
        Sphere *temp;
        if (getIntersection(lightTouch, lDir, objects, &temp) >= (light.loc - lightTouch).length() || temp == NULL) { //Dangerous!
            float tPower = lDir * target.normal(touch);
            if (tPower > 0)
                diffIntense += tPower * light.power;

            Vec3f rDir = getReflection(-lDir, normal);
            tPower = -(rDir * dir);
            if (tPower > 0)
                specIntense += powf(tPower, mat.specular_exponent) * light.power;
        }
    }
    
    return  mat.diffuse_color * diffIntense * mat.albedo[0] +
            WHITE * specIntense * mat.albedo[1] +
            reflColor * mat.albedo[2] +
            refrColor * mat.albedo[3];
}

int main(int argc, char **argv) {
    std::vector<Sphere> objects;
    std::vector<Light> lights;

    objects.push_back(Sphere(Vec3f(0, 0, -14), 3, RED_RUBBER));
    objects.push_back(Sphere(Vec3f(2.5, 2.5, -12.5), 2.25, BLUE_RUBBER));
    objects.push_back(Sphere(Vec3f(-4, 4, -15), 3, MIRROR));

    lights.push_back(Light(Vec3f(10, 25, -1), 3));
    lights.push_back(Light(Vec3f(-6, 5, -6), 2));


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
            image.setPixel(i, j, traceRay(cameraPos, curDir, objects, lights, 0));
        }
    }

    image.write(FILENAME);
    return 0;
}