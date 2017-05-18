#include <iostream>

#include "shape.h"
#include "camera.h"
#include "material.h"
#include "scene.h"

const int WIDTH = 100;
const int HEIGHT = 100;

int main() {
    Camera camera(Vec3(0.0, 0.0, 2.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), 3.0, 3.0);
    camera.initialize();
    Sphere sphere(Vec3(0.0, 0.0, -1.0), 1.0);
    Material material;
    material.set_ambient(0.1);
    material.set_specular(0.0);
    material.set_diffuse(1.0);
    material.set_shininess(32.0);
    material.set_color(Vec3(1.0, 0.0, 0.0));
    Scene scene;
    Object object;
    object.bind_shape(&sphere);
    object.bind_material(&material);
    scene.add_obj(&object);
    PointLight light(Vec3(-1.0, -1.0, 1.0), 1.0);
    scene.add_light(&light);

    FILE *f = fopen("output.txt", "w");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            Ray r = camera.get_ray_through_pixel(i, j, WIDTH, HEIGHT);
            Vec3 color = scene.li(r, 5);
            fprintf(f, "%c ", (char)('a' + (int)(color[0] / 0.1)));
        }
        fprintf(f, "\n");
    }
    fclose(f);

    return 0;
}