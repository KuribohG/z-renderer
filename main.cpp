#include <iostream>

#include "shape.h"
#include "camera.h"
#include "material.h"
#include "scene.h"
#include "kdtree.h"

#include "omp.h"

const int WIDTH = 2000;
const int HEIGHT = 2000;

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
    Camera camera(Vec3(50.0, 30.0, -50.0), Vec3(50.0, 30.0, 50.0), Vec3(0.0, 1.0, 0.0), 100.0, 100.0);
    //Camera camera(Vec3(10.0, 0.0, 3.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), 5.0, 5.0);
    camera.initialize();
    Material mat1, mat2;
    mat1.set_ambient(0.3);
    mat1.set_specular(0.5);
    mat1.set_diffuse(0.6);
    mat1.set_shininess(32.0);
    mat1.set_reflectivity(-1.0);
    mat1.set_color(Vec3(1.0, 1.0, 1.0));
    mat2.set_ambient(0.3);
    mat2.set_specular(0.5);
    mat2.set_diffuse(0.6);
    mat2.set_shininess(32.0);
    mat2.set_reflectivity(-1.0);
    mat2.set_color(Vec3(135.0 / 255, 206.0 / 255, 250.0 / 255));
    Scene scene;
    /*
    Triangle *x = new Triangle(Vec3(0, 0, 100), Vec3(100, 0, 0), Vec3(0, 0, 0));
    Object *obj = new Object();
    obj->bind_shape(x);
    obj->bind_material(&mat1);
    scene.add_obj(obj);
    Triangle *y = new Triangle(Vec3(0, 0, 100), Vec3(100, 0, 100), Vec3(100, 0, 0));
    Object *obj2 = new Object();
    obj2->bind_shape(y);
    obj2->bind_material(&mat2);
    scene.add_obj(obj2);
    */

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int p = i * 10, q = j * 10;
            Triangle *x = new Triangle(Vec3(p, 0, q), Vec3(p, 0, q+10), Vec3(p+10, 0, q));
            Object *obj = new Object();
            obj->bind_shape(x);
            obj->bind_material(&mat1);
            scene.add_obj(obj);
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int p = i * 10, q = j * 10;
            Triangle *x = new Triangle(Vec3(p+10, 0, q+10), Vec3(p+10, 0, q), Vec3(p, 0, q+10));
            Object *obj = new Object();
            obj->bind_shape(x);
            obj->bind_material(&mat2);
            scene.add_obj(obj);
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int p = i * 10, q = j * 10;
            Triangle *x = new Triangle(Vec3(p, 100, q), Vec3(p, 100, q+10), Vec3(p+10, 100, q));
            Object *obj = new Object();
            obj->bind_shape(x);
            obj->bind_material(&mat1);
            scene.add_obj(obj);
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int p = i * 10, q = j * 10;
            Triangle *x = new Triangle(Vec3(p+10, 100, q+10), Vec3(p+10, 100, q), Vec3(p, 100, q+10));
            Object *obj = new Object();
            obj->bind_shape(x);
            obj->bind_material(&mat2);
            scene.add_obj(obj);
        }
    }
    //Sphere *x = new Sphere(Vec3(50.0, 30.0, 50.0), 20.0);
    Material mirror;
    mirror.set_ambient(0.0);
    mirror.set_specular(0.0);
    mirror.set_diffuse(0.0);
    mirror.set_shininess(32.0);
    mirror.set_reflectivity(0.5);
    mirror.set_color(Vec3(0.0, 0.0, 0.0));
    Triangle *x = new Triangle(Vec3(0, 0, 100), Vec3(100, 100, 100), Vec3(0, 100, 100));
    Triangle *y = new Triangle(Vec3(0, 0, 100), Vec3(100, 100, 100), Vec3(100, 0, 100));
    Sphere *z = new Sphere(Vec3(30.0, 30.0, 50.0), 10.0);
    Object *obj = new Object();
    obj->bind_shape(x);
    obj->bind_material(&mirror);
    scene.add_obj(obj);
    Object *obj2 = new Object();
    obj2->bind_shape(y);
    obj2->bind_material(&mirror);
    scene.add_obj(obj2);
    Object *obj3 = new Object();
    obj3->bind_shape(z);
    obj3->bind_material(&mirror);
    scene.add_obj(obj3);
    PointLight light(Vec3(50.0, 50.0, 50.0), 1.0);
    scene.add_light(&light);

    KdTree *tree = new KdTree();
    tree->build(scene.objs);

    unsigned char *data = new unsigned char[WIDTH * HEIGHT * 3];

    #pragma omp parallel for schedule(dynamic, 1) collapse(2)
    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            Ray r = camera.get_ray_through_pixel(i, j, WIDTH, HEIGHT);
            Vec3 color = scene.li(r, 5, tree);
            int id = (HEIGHT - 1 - j) * WIDTH * 3 + i * 3;
            data[id++] = (unsigned char)(std::min(color[0], (Float)1.0) * 256);
            data[id++] = (unsigned char)(std::min(color[1], (Float)1.0) * 256);
            data[id++] = (unsigned char)(std::min(color[2], (Float)1.0) * 256);
        }
    }
    write_image_in_ppm("image.ppm", WIDTH, HEIGHT, data);

    return 0;
}