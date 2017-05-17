#include "object.h"

Object::Object() {}

Shape *Object::get_shape() {
    return shape;
}

Material *Object::get_material() {
    return material;
}

void Object::bind_shape(Shape *source) {
    shape = source;
}

void Object::bind_material(Material *source) {
    material = source;
}
