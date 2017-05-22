#include <iostream>

#include "shape.h"
#include "camera.h"
#include "material.h"
#include "scene.h"

const int WIDTH = 500;
const int HEIGHT = 500;

void write_image_in_ppm(const char *filename, int width, int height, const unsigned char *data) {
    static unsigned char color[3];
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    int id = 0;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            color[0] = data[id++];
            color[1] = data[id++];
            color[2] = data[id++];
            fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);
}

int main() {
    Camera camera(Vec3(0.0, 0.0, 2.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), 1.5, 1.5);
    camera.initialize();
    Triangle triangle(Vec3(-1.0, -1.0, -1.0), Vec3(1.0, -1.0, -1.0), Vec3(-1.0, 1.0, -1.0));
    Triangle another_triangle(Vec3(-1.0, -1.0, -1.0), Vec3(-1.0, -2.0, -1.0), Vec3(0.0, -1.0, -1.0));
    Sphere sphere(Vec3(0.0, 0.0, -1.0), 1.0);
    Sphere another_sphere(Vec3(0.0, 0.0, 5.0), 10.0);
    Material material;
    material.set_ambient(0.0);
    material.set_specular(0.0);
    material.set_diffuse(0.0);
    material.set_shininess(32.0);
    material.set_reflectivity(1.0);
    material.set_color(Vec3(1.0, 0.0, 0.0));
    Material another_material;
    another_material.set_ambient(0.1);
    another_material.set_specular(0.5);
    another_material.set_diffuse(0.2);
    another_material.set_shininess(32.0);
    another_material.set_reflectivity(0.0);
    another_material.set_color(Vec3(0.0, 1.0, 0.0));
    Scene scene;
    Object object;
    object.bind_shape(&triangle);
    object.bind_material(&another_material);
    scene.add_obj(&object);
    Object another_object;
    another_object.bind_shape(&another_triangle);
    another_object.bind_material(&another_material);
    scene.add_obj(&another_object);
    PointLight light(Vec3(-1.0, -1.0, 1.0), 1.0);
    scene.add_light(&light);

    unsigned char *data = new unsigned char[WIDTH * HEIGHT * 3];
    int id = 0;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            Ray r = camera.get_ray_through_pixel(i, j, WIDTH, HEIGHT);
            Vec3 color = scene.li(r, 5);
            data[id++] = (unsigned char)(std::min(color[0], (Float)1.0) * 256);
            data[id++] = (unsigned char)(std::min(color[1], (Float)1.0) * 256);
            data[id++] = (unsigned char)(std::min(color[2], (Float)1.0) * 256);
        }
    }
    write_image_in_ppm("image.ppm", WIDTH, HEIGHT, data);

    return 0;
}