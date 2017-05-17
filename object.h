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
};

#endif //RENDER_OBJECT_H
