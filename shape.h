#ifndef RENDER_SHAPE_H
#define RENDER_SHAPE_H

#include "base.h"
#include "intersection.h"

class Shape {
public:
    virtual bool intersect(const Ray &r) const = 0;
    virtual void intersect_point(const Ray &r, Intersection &isect) const = 0;
};

class Sphere : public Shape {
private:
    Vec3 position;
    Float radius;
public:
    Sphere(const Vec3 &position, Float radius);
    virtual bool intersect(const Ray &r) const;
    virtual void intersect_point(const Ray &r, Intersection &isect) const;
};

class Triangle : public Shape {
private:
    //Vec3 p[3];
public:
    Vec3 p[3];
    Triangle(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2);
    virtual bool intersect(const Ray &r) const;
    virtual void intersect_point(const Ray &r, Intersection &isect) const;
};

#endif //RENDER_SHAPE_H
