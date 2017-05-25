#include <limits>
#include <algorithm>

#include "bbox.h"

BBox::BBox() {
    Float min_num = std::numeric_limits<Float>::lowest();
    Float max_num = std::numeric_limits<Float>::max();
    min_v = Vec3(max_num, max_num, max_num);
    max_v = Vec3(min_num, min_num, min_num);
}

BBox::BBox(const Vec3 &min_v, const Vec3 &max_v)
        : min_v(min_v), max_v(max_v) {}

void BBox::expand(const BBox &p) {
    for (int i = 0; i < 3; i++) {
        min_v.set(i, std::min(min_v[i], p.min_v[i]));
        max_v.set(i, std::max(max_v[i], p.max_v[i]));
    }
}

Vec3 BBox::midpoint() const {
    return (min_v + max_v) / 2.0;
}

// http://www.cs.utah.edu/~awilliam/box/box.pdf
bool BBox::hit(const Ray &r) const {
    Float tmin, tmax, tymin, tymax, tzmin, tzmax;
    Vec3 origin = r.get_origin(), direction = r.get_direction();
    if (direction[0] >= 0) {
        tmin = (min_v[0] - origin[0]) / direction[0];
        tmax = (max_v[0] - origin[0]) / direction[0];
    } else {
        tmin = (max_v[0] - origin[0]) / direction[0];
        tmax = (min_v[0] - origin[0]) / direction[0];
    }
    if (direction[1] >= 0) {
        tymin = (min_v[1] - origin[1]) / direction[1];
        tymax = (max_v[1] - origin[1]) / direction[1];
    } else {
        tymin = (max_v[1] - origin[1]) / direction[1];
        tymax = (min_v[1] - origin[1]) / direction[1];
    }
    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    if (direction[2] >= 0) {
        tzmin = (min_v[2] - origin[2]) / direction[2];
        tzmax = (max_v[2] - origin[2]) / direction[2];
    } else {
        tzmin = (max_v[2] - origin[2]) / direction[2];
        tzmax = (min_v[2] - origin[2]) / direction[2];
    }
    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    return tmax > -eps;
}