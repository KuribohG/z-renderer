#ifndef RENDER_INTERSECTION_H
#define RENDER_INTERSECTION_H

#include "base.h"

class Intersection {
private:
    Vec3 point;
    Vec3 normal;
public:
    Intersection();
    Vec3 get_point() const;
    Vec3 get_normal() const;
    void set_point(const Vec3 &source);
    void set_normal(const Vec3 &source);
};

#endif //RENDER_INTERSECTION_H
