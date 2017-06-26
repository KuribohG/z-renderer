#ifndef RENDER_SHAPE_H
#define RENDER_SHAPE_H

#include "base.h"
#include "intersection.h"
#include "bbox.h"

class Shape {
public:
    virtual bool intersect(const Ray &r) const = 0;
    virtual void intersect_point(const Ray &r, Intersection &isect) const = 0;
    virtual BBox get_bbox() const = 0;
    virtual Vec3 surface_normal(const Vec3 &p) const = 0;
};

class Sphere : public Shape {
private:
    Vec3 position;
    Float radius;
public:
    Sphere(const Vec3 &position, Float radius);
    virtual bool intersect(const Ray &r) const;
    virtual void intersect_point(const Ray &r, Intersection &isect) const;
    virtual BBox get_bbox() const;
    virtual Vec3 surface_normal(const Vec3 &p) const;
};

class Triangle : public Shape {
public:
    Vec3 p[3];
    Triangle(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2);
    virtual bool intersect(const Ray &r) const;
    virtual void intersect_point(const Ray &r, Intersection &isect) const;
    virtual BBox get_bbox() const;
    virtual Vec3 surface_normal(const Vec3 &p) const;
};

class TriangleMesh {
public:
    int n_vertices, n_triangles;
    int *faces_v, *faces_vt, *faces_vn;
    Vec3 *v, *vt, *vn;
    TriangleMesh(int n_vertices, int n_vertice_textures, int n_vertice_normals, int n_triangles);
    ~TriangleMesh();
};

enum MeshTriangleAttribute {HAS_TEXTURE, HAS_NORMAL};

class MeshTriangle : public Shape {
public:
    TriangleMesh *mesh;
    int state;
    int *start_v, *start_vt, *start_vn;
public:
    MeshTriangle(TriangleMesh *mesh, int id);
    bool check_attribute(MeshTriangleAttribute attr) const;
    virtual bool intersect(const Ray &r) const;
    virtual void intersect_point(const Ray &r, Intersection &isect) const;
    virtual BBox get_bbox() const;
    virtual Vec3 surface_normal(const Vec3 &p) const;
};

#endif //RENDER_SHAPE_H
