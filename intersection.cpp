#include "intersection.h"

Intersection::Intersection() {}

void Intersection::set_normal(const Vec3 &source) {
    normal = source.normalize();
}
