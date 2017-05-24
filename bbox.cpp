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
