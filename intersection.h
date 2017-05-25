#ifndef RENDER_INTERSECTION_H
#define RENDER_INTERSECTION_H

#include "base.h"

class Intersection {
public:
    Vec3 point;
    Vec3 normal;
    Intersection();
    void set_normal(const Vec3 &source);
};

#endif //RENDER_INTERSECTION_H
