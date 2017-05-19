#include "intersection.h"

Intersection::Intersection() {}

Vec3 Intersection::get_point() const {
    return point;
}

Vec3 Intersection::get_normal() const {
    return normal;
}

void Intersection::set_point(const Vec3 &source) {
    point = source;
}

void Intersection::set_normal(const Vec3 &source) {
    normal = source.normalize();
}
