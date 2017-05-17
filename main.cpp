#include <iostream>

#include "shape.h"
#include "camera.h"

const int WIDTH = 100;
const int HEIGHT = 100;

int main() {
    Camera camera(Vec3(0.0, 0.0, 2.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), 3.0, 3.0);
    camera.initialize();
    Sphere sphere(Vec3(0.0, 0.0, -1.0), 1.0);
    FILE *f = fopen("output.txt", "w");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            Ray r = camera.get_ray_through_pixel(i, j, WIDTH, HEIGHT);
            if (sphere.intersect(r)) {
                fprintf(f, "1 ");
            } else {
                fprintf(f, "0 ");
            }
        }
        fprintf(f, "\n");
    }

    return 0;
}