#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "shape.h"
#include "material.h"

class Object {
private:
    Shape *shape;
    Material *material;
public:
    Object();
    Shape *get_shape();
    Material *get_material();
    void bind_shape(Shape *source);
    void bind_material(Material *source);
    void intersect_point(const Ray &r, Intersection &isect) const;
};

#endif //RENDER_OBJECT_H
