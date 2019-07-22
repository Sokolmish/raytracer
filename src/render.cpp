#include "render.hpp"

Intersection Render::getIntersection(const Vec3f &origin, const Vec3f &dir, const Scene &scene, bool needData) {
    if (scene.getKDTree().isIntersect(origin, dir))
        return scene.getKDTree().getIntersection(origin, dir, needData);
    return Intersection(false, 0.f);
}

Color Render::traceRay(const Vec3f &origin, const Vec3f &dir, const Scene &scene, int depth) {
    if (depth >= max_ray_depth)
        return env.getColor(origin, dir);

    Intersection inter = getIntersection(origin, dir, scene, true);
    if (!inter.state)
        return env.getColor(origin, dir);

    Vec3f touch = inter.touch;
    Vec3f normal = inter.normal;
    Material mat = inter.mat;

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
    for (auto&& light : scene.getLights()) {
        Vec3f lDir = (light.loc - touch).normalize(); //Light direction
        Vec3f lightTouch = getNearPoint(touch, lDir, normal);
        
        Intersection lInter = getIntersection(lightTouch, lDir, scene, false);
        if (!lInter.state || lInter.len >= (light.loc - lightTouch).length()) {
            float tPower = lDir * normal;
            if (tPower > 0)
                diffIntense += tPower * light.power;

            Vec3f rDir = getReflection(-lDir, normal);
            tPower = -(rDir * dir);
            if (tPower > 0)
                specIntense += powf(tPower, mat.specular_exponent) * light.power;
        }
    }
    
    return  mat.diffuse_color * diffIntense * mat.albedo[0] +
            Vec3f(1.f, 1.f, 1.f) * specIntense * mat.albedo[1] +
            reflColor * mat.albedo[2] +
            refrColor * mat.albedo[3];
}

void Render::render(Image &image, const Camera &camera, const Scene &scene) {
    float xSh = -camera.width / 2;
    float ySh = -camera.height / 2;
    // #pragma omp parallel for collapse(2) //Temporary slow down the program ¯\_(ツ)_/¯
    for (int j = 0; j < camera.height; j++) {
        for (int i = 0; i < camera.width; i++) {
            Vec3f curDir = (camera.dir * camera.depth + camera.up * (ySh + j) + camera.right * (xSh + i)).normalize();
            image.setPixel(i, j, traceRay(camera.pos, curDir, scene, 0));
        }
    }
    antiAlliasing(image, camera, scene);
}

void Render::antiAlliasing(Image &image, const Camera &camera, const Scene &scene) {
    float xSh = -camera.width / 2;
    float ySh = -camera.height / 2;
    uint8_t *grayImg = new uint8_t[camera.width * camera.height];
    #pragma omp parallel for collapse(2)
    for (int j = 0; j < camera.height; j++) {
        for (int i = 0; i < camera.width; i++) {
            Color col = image.getPixel(i, j) * 255.f; //r = x, g = y, b = z
            grayImg[j * camera.width + i] = (uint8_t)(0.2126f * col.x + 0.7152f * col.y + 0.0722f * col.z);
        }
    }
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}}; //Sobel kernel
    int gx[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int *sobel = new int[camera.width * camera.height];
    #pragma omp parallel for collapse(2)
    for (int j = 1; j < camera.height - 1; j++) {
        for (int i = 1; i < camera.width - 1; i++) {
            int nx = 0, ny = 0;
            for (int tx = -1; tx <= 1; tx++) {
                for (int ty = -1; ty <= 1; ty++) {
                    nx += gx[ty + 1][tx + 1] * grayImg[(j + ty) * camera.width + i + tx];
                    ny += gy[ty + 1][tx + 1] * grayImg[(j + ty) * camera.width + i + tx];
                }
            }
            sobel[j * camera.width + i] = sqrtf(nx * nx + ny * ny);
        }
    }
    const float pix = 0.4f; //defines deviation of additional rays)
    // #pragma omp parallel for collapse(2) //See above
    for (int j = 0; j < camera.height; j++) {
        for (int i = 0; i < camera.width; i++) {
            if (sobel[j * camera.width + i] > ALLIASING_EDGE) {
                Vec3f curDir = (camera.dir * camera.depth + camera.up * (ySh + j) + camera.right * (xSh + i));
                Color oldColor = image.getPixel(i, j);
                Color lbCol = traceRay(camera.pos, (curDir - pix * camera.right - pix * camera.up).normalize(), scene, 0);
                Color ltCol = traceRay(camera.pos, (curDir - pix * camera.right + pix * camera.up).normalize(), scene, 0);
                Color rbCol = traceRay(camera.pos, (curDir + pix * camera.right - pix * camera.up).normalize(), scene, 0);
                Color rtCol = traceRay(camera.pos, (curDir + pix * camera.right + pix * camera.up).normalize(), scene, 0);
                Color average = (oldColor + lbCol + ltCol + rbCol + rbCol) / 5.f;
                image.setPixel(i, j, average);
            }
        }
    }
    delete[] grayImg;
    delete[] sobel;
}