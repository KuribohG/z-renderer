#ifndef RENDER_INTERSECTION_H
#define RENDER_INTERSECTION_H

#include "base.h"

class Intersection {
public:
    Vec3 point;
    Vec3 normal;
    Vec3 barycentric; // for triangles
    Vec3 texture_coord;
    Vec3 color;
    Intersection();
    void set_normal(const Vec3 &source);
};

#endif //RENDER_INTERSECTION_H
