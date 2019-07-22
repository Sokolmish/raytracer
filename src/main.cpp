#include <iostream>
#include <ctime>

#include "render.hpp"
#include "image.hpp"

#define FILENAME "output.bmp"

#define MAX_RAY_DEPTH 6

#define MAX_KD_DEPTH 10
#define MAX_KD_LEAF 5
#define KD_PARTS 10

void fillScene(Scene &scene) {
    scene.addObjModel(Vec3f(0, 0, 0), MAT_SCALE(45, 45, 45), "input/env.obj");
    scene.addObjModel(Vec3f(7, 0, -13), MAT_SCALE(2e-2, 2e-2, 2e-2) * MAT_ROT_Y(toRad(-135)), "input/cat.obj");

    // scene.addSphere(Vec3f(-5, 3.5, -20), 3.5, RED_RUBBER);
    // scene.addSphere(Vec3f(-3, 7, -18), 2.5, BLUE_RUBBER);

    // scene.addObjModel(Vec3f(-5, 0, -15), MAT_SCALE(10, 10, 10), "input/test1.obj");

    scene.addLight(Vec3f(0, 50, 0), 1.5);
    scene.addLight(Vec3f(0, 7, 5), 2);
}

int main() { //3.19 seconds
    Camera camera(
        RES_720p,           //Resolution
        Vec3f(0, 7, 5),     //Position
        Vec3f(0, 0, -1),    //Direction of view
        Vec3f(0, 1, 0),     //Vertical direction
        toRad(90)           //FOV
    );
    Environment env(    
        LIGHT_BLUE          //Background
    );
    Scene scene;
    fillScene(scene);

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
    image.free();
    return 0;
}