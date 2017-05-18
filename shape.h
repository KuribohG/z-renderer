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
    Vec3 get_normal(const Vec3 &p) const;
    virtual bool intersect(const Ray &r);
    Vec3 intersect_point(const Ray &r);
};

#endif //RENDER_SHAPE_H
