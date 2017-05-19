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
    Sphere(Vec3 position, Float radius);
    virtual bool intersect(const Ray &r) const;
    void intersect_point(const Ray &r, Intersection &isect) const;
};

#endif //RENDER_SHAPE_H
