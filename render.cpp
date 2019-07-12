#include "render.hpp"

float Render::getIntersection(const Vec3f &origin, const Vec3f &dir, const Scene &scene, VolumeObj **out) {
    if (scene.getKDTree().isIntersect(origin, dir))
        return scene.getKDTree().getIntersection(origin, dir, out);
    *out = NULL;
    return 0;
}

Color Render::traceRay(const Vec3f &origin, const Vec3f &dir, const Scene &scene, int depth) {
    if (depth >= max_ray_depth)
        return env.getColor(origin, dir);

    VolumeObj *_target;
    float tLen = getIntersection(origin, dir, scene, &_target);

    if (_target == NULL)
        return env.getColor(origin, dir);

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
    for (auto&& light : scene.getLights()) {
        Vec3f lDir = (light.loc - touch).normalize(); //Light direction
        Vec3f lightTouch = getNearPoint(touch, lDir, normal);
        VolumeObj *temp;
        if (getIntersection(lightTouch, lDir, scene, &temp) >= (light.loc - lightTouch).length() || temp == NULL) { //Dangerous!
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

    //Antialliasing
    uint8_t *grayImg = new uint8_t[camera.width * camera.height];
    for (int j = 0; j < camera.height; j++) {
        for (int i = 0; i < camera.width; i++) {
            Color col = image.getPixel(i, j); //r = x, g = y, b = z
            grayImg[j * camera.width + i] = (uint8_t)(0.2126f * col.x + 0.7152f * col.y + 0.0722f * col.z);
        }
    }
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}}; //Sobel kernel
    int gx[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int *sobel = new int[camera.width * camera.height];
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
