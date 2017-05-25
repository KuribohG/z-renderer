#include <iostream>

#include "shape.h"
#include "camera.h"
#include "material.h"
#include "scene.h"
#include "kdtree.h"

#include "omp.h"

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

void load_obj(const char *filename, std::vector<Triangle> &mesh) {
    FILE *fp = fopen(filename, "r");
    char type[10];
    std::vector<Vec3> vertices;
    while(true) {
        int res = fscanf(fp, "%s", type);
        if (res == EOF) {
            break;
        }
        if (strcmp(type, "v") == 0) {
            Float x, y, z;
            fscanf(fp, "%f%f%f", &x, &y, &z);
            vertices.emplace_back(x, y, z);
        } else if(strcmp(type, "f") == 0) {
            int x, y, z;
            fscanf(fp, "%d%d%d", &x, &y, &z);
            x--, y--, z--;
            mesh.emplace_back(vertices[x], vertices[y], vertices[z]);
        }
    }
    fclose(fp);
}

int main() {
    Camera camera(Vec3(30.0, 32.0, 32.0), Vec3(0.0, 32.0, 32.0), Vec3(0.0, 0.0, 1.0), 100.0, 100.0);
    camera.initialize();
    Material material;
    material.set_ambient(0.1);
    material.set_specular(0.3);
    material.set_diffuse(0.5);
    material.set_shininess(16.0);
    material.set_reflectivity(-1.0);
    material.set_color(Vec3(1.0, 1.0, 1.0));
    Scene scene;
    std::vector<Triangle> mesh;
    load_obj("example1.obj", mesh);
    Object *objs = new Object[(int)mesh.size()];
    for (int i = 0; i < mesh.size(); i++) {
        objs[i].bind_shape(&mesh[i]);
        objs[i].bind_material(&material);
        scene.add_obj(&objs[i]);
    }
    PointLight light(Vec3(31.0, 32.0, 32.0), 1.0);
    scene.add_light(&light);

    KdTree *tree = new KdTree();
    tree->build(scene.objs);

    unsigned char *data = new unsigned char[WIDTH * HEIGHT * 3];

    #pragma omp parallel for schedule(dynamic, 1) collapse(2)
    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            Ray r = camera.get_ray_through_pixel(i, j, WIDTH, HEIGHT);
            Vec3 color = scene.li(r, 5);
            int id = (HEIGHT - 1 - j) * WIDTH * 3 + i * 3;
            data[id++] = (unsigned char)(std::min(color[0], (Float)1.0) * 256);
            data[id++] = (unsigned char)(std::min(color[1], (Float)1.0) * 256);
            data[id++] = (unsigned char)(std::min(color[2], (Float)1.0) * 256);
        }
    }
    write_image_in_ppm("image.ppm", WIDTH, HEIGHT, data);

    delete [] objs;

    return 0;
}