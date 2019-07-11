#include <iostream>
#include <omp.h>
#include <ctime>

// #include <GL/freeglut.h>

#include "render.hpp"
#include "image.hpp"

#define FILENAME "./output/output.bmp"

#define MAX_RAY_DEPTH 6

#define MAX_KD_DEPTH 10
#define MAX_KD_LEAF 5
#define KD_PARTS 10

int main() {
    Camera camera(
        RES_720p,           //Resolution
        Vec3f(3, 4, 3),     //Position
        Vec3f(0, 0, -1),    //Direction of view
        Vec3f(0, 1, 0),     //Vertical direction
        toRad(90)           //FOV
    );
    Environment env(
        LIGHT_BLUE          //Background
    );
    Scene scene;
    

    scene.addObject(new Sphere(Vec3f(0, 0, -14), 3, RED_RUBBER));
    scene.addObject(new Sphere(Vec3f(2.5, 2.5, -12.5), 2.25, BLUE_RUBBER));
    scene.addObject(new Sphere(Vec3f(-4, 5, -15), 3, MIRROR));

    scene.addObject(createQuadrangle(Vec3f(-8, -3, -9), Vec3f(15, -3, -9), Vec3f(15, -3, -20), Vec3f(-8, -3, -20), GREEN_RUBBER));
    scene.addObject(createQuadrangle(Vec3f(-8, -3, -9), Vec3f(-8, -3, -20), Vec3f(-8, 9, -20), Vec3f(-8, 9, -9), MIRROR));
    scene.addObject(createQuadrangle(Vec3f(-8, -3, -20), Vec3f(15, -3, -20), Vec3f(15, 9, -20), Vec3f(-8, 9, -20), MIRROR));

    scene.addObject(createSerpinsky(3, Vec3f(9, 4.05, -13), 7, 6, toRad(90), CYAN_RUBBER));
    // scene.addObject(createPyramid(Vec3f(9, 4.05, -13), 7, 6, toRad(90), CYAN_RUBBER));

    scene.addLight(Light(Vec3f(10, 25, -1), 3));
    scene.addLight(Light(Vec3f(-6, 5, -6), 2));
    scene.addLight(Light(Vec3f(5, 5, -15), 0.5));


    uint64_t time = clock();
    scene.buildKDtree(MAX_KD_DEPTH, MAX_KD_LEAF, KD_PARTS);
    std::cout << "KD-tree building: " << (clock() - time) / (float) CLOCKS_PER_SEC << " seconds." << std::endl;

    Render render(env, MAX_RAY_DEPTH);
    Image image(camera.width, camera.height);
    
    time = clock();
    render.render(image, camera, scene);
    std::cout << "Rendering: " << (clock() - time) / (float) CLOCKS_PER_SEC << " seconds." << std::endl;

    image.write(FILENAME);
    scene.free();
    return 0;
}