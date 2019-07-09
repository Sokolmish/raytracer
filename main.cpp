#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

// #include <GL/freeglut.h>

#include "image.hpp"
#include "util.hpp"
#include "objects.hpp"

#define FILENAME "./output/output.bmp"

#define BACKGROUND LIGHT_BLUE //Later background will be changed to skybox
#define MAX_DEPTH 5

float getIntersection(const Vec3f &origin, const Vec3f &dir, const std::vector<VolumeObj*> &objects, VolumeObj **out) {
    *out = NULL;
    float len = __FLT_MAX__;
    for (auto&& obj : objects) {
        float t = obj->intersect(origin, dir);
        if (t >= 0 && t < len) {
            *out = obj;
            len = t;
        }
    }
    return len;
}

Color traceRay(const Vec3f &origin, const Vec3f &dir, const Scene &scene, int depth) {
    if (depth >= MAX_DEPTH)
        return BACKGROUND;

    VolumeObj *_target;
    float tLen = getIntersection(origin, dir, scene.objects, &_target);

    if (_target == NULL)
        return BACKGROUND;

    VolumeObj &target = *_target;
    Vec3f touch = origin + tLen * dir;
    Vec3f normal = target.normal(touch);
    Material mat = target.material(touch);

    Color reflColor = BLACK, refrColor = BLACK; //Black don't affect to color in addition
    if (mat.albedo[2] >= 1e-3) {
        Vec3f reflDir = getReflection(dir, normal);
        Vec3f reflTouch = getNearPoint(touch, reflDir, normal);
        reflColor = traceRay(reflTouch, reflDir, scene, depth + 1);
    }
    if (mat.albedo[3] >= 1e-3) {
        Vec3f refrDir = getRefraction(dir, normal, mat.refractive_index);
        Vec3f refrTouch = getNearPoint(touch, refrDir, normal);
        refrColor = traceRay(refrTouch, refrDir, scene, depth + 1);
    }
    
    float diffIntense = 0.f;
    float specIntense = 0.f;
    for (auto&& light : scene.lights) {
        Vec3f lDir = (light.loc - touch).normalize(); //Light direction
        Vec3f lightTouch = getNearPoint(touch, lDir, normal);
        VolumeObj *temp;
        if (getIntersection(lightTouch, lDir, scene.objects, &temp) >= (light.loc - lightTouch).length() || temp == NULL) { //Dangerous!
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

void render(Image &image, const Camera &camera, const Scene &scene) {
    float xSh = -camera.width / 2;
    float ySh = -camera.height / 2;
    #pragma omp parallel for
    for (int j = 0; j < camera.height; j++) {
        for (int i = 0; i < camera.width; i++) {
            Vec3f curDir = (camera.dir * camera.depth + camera.up * (ySh + j) + camera.right * (xSh + i)).normalize();
            image.setPixel(i, j, traceRay(camera.pos, curDir, scene, 0));
        }
    }
}

void addTriangles(std::vector<VolumeObj*> &dest, const std::vector<Triangle*> &src) {
    for (auto&& e : src)
        dest.push_back(e);
}

int main(int argc, char **argv) {
    uint64_t time = clock();
    Scene scene;

    Camera camera(
        RES_HD              //Resolution
        Vec3f(3, 4, 3),     //Position
        Vec3f(0, 0, -1),    //Direction of view
        Vec3f(0, 1, 0),     //Vertical direction
        toRad(90)           //FOV
    );

    // scene.objects.push_back(new Sphere(Vec3f(5, 0, -15), 0.1, IVORY));

    scene.objects.push_back(new Sphere(Vec3f(0, 0, -14), 3, RED_RUBBER));
    scene.objects.push_back(new Sphere(Vec3f(2.5, 2.5, -12.5), 2.25, BLUE_RUBBER));
    scene.objects.push_back(new Sphere(Vec3f(-4, 5, -15), 3, MIRROR));

    addTriangles(scene.objects, createQuadrangle(Vec3f(-8, -3, -9), Vec3f(15, -3, -9), Vec3f(15, -3, -20), Vec3f(-8, -3, -20), GREEN_RUBBER));
    addTriangles(scene.objects, createQuadrangle(Vec3f(-8, -3, -9), Vec3f(-8, -3, -20), Vec3f(-8, 9, -20), Vec3f(-8, 9, -9), MIRROR));
    addTriangles(scene.objects, createQuadrangle(Vec3f(-8, -3, -20), Vec3f(15, -3, -20), Vec3f(15, 9, -20), Vec3f(-8, 9, -20), MIRROR));

    addTriangles(scene.objects, createSerpinsky(2, Vec3f(9, 4.05, -13), 7, 6, toRad(90), CYAN_RUBBER));

    scene.lights.push_back(Light(Vec3f(10, 25, -1), 3));
    scene.lights.push_back(Light(Vec3f(-6, 5, -6), 2));
    scene.lights.push_back(Light(Vec3f(5, 5, -15), 0.5));

    Image image(camera.width, camera.height);
    render(image, camera, scene);
    std::cout << "Time: " << (clock() - time) / (float) CLOCKS_PER_SEC << " seconds." << std::endl;
    image.write(FILENAME);

    for (auto&& t : scene.objects)
        delete t;
    return 0;
}