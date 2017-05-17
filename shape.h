#ifndef RENDER_SHAPE_H
#define RENDER_SHAPE_H

#include "base.h"

class Shape {
public:
    virtual bool intersect(const Ray &r) = 0;
    virtual Vec3 intersect_point(const Ray &r) = 0;
};

class Sphere : public Shape {
private:
    Vec3 position;
    Float radius;
public:
    Sphere(Vec3 position, Float radius);
    virtual bool intersect(const Ray &r);
    Vec3 intersect_point(const Ray &r);
};

#endif //RENDER_SHAPE_H
