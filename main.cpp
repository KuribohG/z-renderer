#include <iostream>
#include <tuple>
#include <cassert>

#include "shape.h"
#include "camera.h"
#include "material.h"
#include "scene.h"
#include "kdtree.h"
#include "integrator.h"

#include "omp.h"

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

std::tuple<int, int, int> convert(char *str) {
    int len = strlen(str);
    int c = 0, start[3] = {0};
    for (int i = 0; i < len; i++) {
        if (str[i] == '/') {
            c++, str[i] = 0;
            start[c] = i + 1;
        }
    }
    for (int i = c + 1; i < 3; i++) {
        start[i] = len;
    }
    int ans[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        if (str[start[i]] == 0) continue;
        else ans[i] = atoi(str + start[i]);
    }
    return std::tuple<int, int, int>{ans[0] - 1, ans[1] - 1, ans[2] - 1};
}

int get_elem_i(std::tuple<int, int, int> t, int i) {
    switch (i) {
        case 0: return std::get<0>(t);
        case 1: return std::get<1>(t);
        case 2: return std::get<2>(t);
    }
    return -2;
}

void load_obj(const char *filename, std::vector<MeshTriangle*> &triangles) {
    FILE *fp = fopen(filename, "r");
    char type[10];
    std::vector<Vec3> v, vt, vn;
    std::vector<std::tuple<int, int, int> > f_v, f_vt, f_vn;
    while(true) {
        int res = fscanf(fp, "%s", type);
        if (res == EOF) {
            break;
        }
        if (strcmp(type, "v") == 0) {
            Float x, y, z;
            fscanf(fp, "%f%f%f", &x, &y, &z);
            v.emplace_back(x, y, z);
        } else if (strcmp(type, "vt") == 0) {
            Float x, y, z;
            fscanf(fp, "%f%f%f", &x, &y, &z);
            vt.emplace_back(x, y, z);
        } else if (strcmp(type, "vn") == 0) {
            Float x, y, z;
            fscanf(fp, "%f%f%f", &x, &y, &z);
            vn.emplace_back(x, y, z);
        } else if (strcmp(type, "f") == 0) {
            char x[20], y[20], z[20];
            fscanf(fp, "%s%s%s", x, y, z);
            std::tuple<int, int, int> xx = convert(x);
            std::tuple<int, int, int> yy = convert(y);
            std::tuple<int, int, int> zz = convert(z);
            f_v.push_back(std::make_tuple(std::get<0>(xx), std::get<0>(yy), std::get<0>(zz)));
            f_vt.push_back(std::make_tuple(std::get<1>(xx), std::get<1>(yy), std::get<1>(zz)));
            f_vn.push_back(std::make_tuple(std::get<2>(xx), std::get<2>(yy), std::get<2>(zz)));
        }
    }
    fclose(fp);
    int n = (int)v.size(), m = (int)f_v.size(), nn = (int)vn.size(), nt = (int)vt.size();
    TriangleMesh *mesh = new TriangleMesh(n, nt, nn, m);
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < 3; k++) {
            mesh->faces_v[i * 3 + k] = get_elem_i(f_v[i], k);
            mesh->faces_vt[i * 3 + k] = get_elem_i(f_vt[i], k);
            mesh->faces_vn[i * 3 + k] = get_elem_i(f_vn[i], k);
        }
    }
    for (int i = 0; i < n; i++) mesh->v[i] = v[i];
    for (int i = 0; i < nt; i++) mesh->vt[i] = vt[i];
    for (int i = 0; i < nn; i++) mesh->vn[i] = vn[i];
    for (int i = 0; i < m; i++) {
        MeshTriangle *x = new MeshTriangle(mesh, i);
        triangles.push_back(x);
    }
}

void test1() {
    const int WIDTH = 2000;
    const int HEIGHT = 2000;
    Camera camera(Vec3(50.0, 30.0, -50.0), Vec3(50.0, 30.0, 50.0), Vec3(0.0, 1.0, 0.0), 100.0, 100.0);
    camera.initialize();
    PhongMaterial mat1, mat2;
    mat1.ambient = 0.3;
    mat1.specular = 0.5;
    mat1.diffuse = 0.6;
    mat1.shininess = 32.0;
    mat1.reflectivity = -1.0;
    mat1.set_color(Vec3(1.0, 1.0, 1.0));
    mat2.ambient = 0.3;
    mat2.specular = 0.5;
    mat2.diffuse = 0.6;
    mat2.shininess = 32.0;
    mat2.reflectivity = -1.0;
    mat2.set_color(Vec3(135.0 / 255, 206.0 / 255, 250.0 / 255));
    Scene scene;

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

    PhongMaterial mirror;
    mirror.ambient = 0.3;
    mirror.specular = 0.0;
    mirror.diffuse = 0.0;
    mirror.shininess = 32.0;
    mirror.reflectivity = 0.5;
    mirror.set_color(Vec3(1.0, 0.0, 0.0));
    PhongMaterial mirror1;
    mirror1.ambient = 0.0;
    mirror1.specular = 0.0;
    mirror1.diffuse = 0.0;
    mirror1.shininess = 32.0;
    mirror1.reflectivity = 0.5;
    mirror1.set_color(Vec3(0.0, 0.0, 0.0));
    Triangle *x = new Triangle(Vec3(0, 0, 100), Vec3(100, 100, 100), Vec3(0, 100, 100));
    Triangle *y = new Triangle(Vec3(0, 0, 100), Vec3(100, 100, 100), Vec3(100, 0, 100));
    Sphere *z = new Sphere(Vec3(30.0, 30.0, 50.0), 10.0);
    Sphere *w = new Sphere(Vec3(70.0, 30.0, 50.0), 10.0);
    Object *obj = new Object();
    obj->bind_shape(x);
    obj->bind_material(&mirror1);
    scene.add_obj(obj);
    Object *obj2 = new Object();
    obj2->bind_shape(y);
    obj2->bind_material(&mirror1);
    scene.add_obj(obj2);
    Object *obj3 = new Object();
    obj3->bind_shape(z);
    obj3->bind_material(&mirror);
    scene.add_obj(obj3);
    Object *obj4 = new Object();
    obj4->bind_shape(w);
    obj4->bind_material(&mirror);
    scene.add_obj(obj4);
    PointLight light(Vec3(50.0, 50.0, 50.0), 1.0);
    scene.add_light(&light);

    KdTree *tree = new KdTree();
    tree->build(scene.objs);

    unsigned char *data = new unsigned char[WIDTH * HEIGHT * 3];

    WhittedIntegrator integrator;
    integrator.render(data, HEIGHT, WIDTH, &camera, &scene, 5, tree);

    write_image_in_ppm("image.ppm", WIDTH, HEIGHT, data);
}

void test_mesh() {
    const int HEIGHT = 500;
    const int WIDTH = 500;

    Scene scene;
    std::vector<MeshTriangle*> v;
    load_obj("example.obj", v);

    PhongMaterial mat;
    mat.ambient = 0.1;
    mat.specular = 0.0;
    mat.diffuse = 0.7;
    mat.shininess = 16.0;
    mat.reflectivity = -1.0;
    mat.set_color(Vec3(1.0, 1.0, 1.0));
    for (MeshTriangle *tri : v) {
        Object *obj = new Object();
        obj->bind_shape(tri);
        obj->bind_material(&mat);
        scene.add_obj(obj);
    }

    Camera camera(Vec3(30.0, 32.0, 32.0), Vec3(0.0, 32.0, 32.0), Vec3(0.0, 0.0, 1.0), 100.0, 100.0);
    camera.initialize();

    PointLight light(Vec3(31.0, 32.0, 32.0), 1.0);
    scene.add_light(&light);

    KdTree *tree = new KdTree();
    tree->build(scene.objs);

    unsigned char *data = new unsigned char[WIDTH * HEIGHT * 3];

    WhittedIntegrator integrator;
    integrator.render(data, HEIGHT, WIDTH, &camera, &scene, 5, tree);

    write_image_in_ppm("image.ppm", WIDTH, HEIGHT, data);
}

int main() {
    test1();
    //test_mesh();

    return 0;
}