#ifndef RENDER_SHAPE_H
#define RENDER_SHAPE_H

#include "base.h"

class Shape {};

class Sphere : public Shape {
private:
    Vec3 position;
    Float radius;
public:
    Sphere(Vec3 position, Float radius);
    bool intersect(const Ray &r);
    Vec3 intersect_point(const Ray &r);
};

#endif //RENDER_SHAPE_H
