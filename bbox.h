#ifndef RENDER_BBOX_H
#define RENDER_BBOX_H

#include "base.h"

class BBox {
public:
    Vec3 min_v, max_v;
    BBox();
    BBox(const Vec3 &min_v, const Vec3 &max_v);
    void expand(const BBox &p);
    Vec3 midpoint() const;
};

#endif //RENDER_BBOX_H
