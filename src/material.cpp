#include "material.h"

Material::Material(MaterialType type)
        : type(type) {}

PhongMaterial::PhongMaterial()
        : Material(PHONG) {}

PhongMaterial::~PhongMaterial() {}

void PhongMaterial::set_color(const Vec3 &color) {
    texture = new ConstantTexture2D<Vec3>(color);
}

void PhongMaterial::complete_intersection(Intersection &isect) const {
    isect.color = texture->at(isect.texture_coord[0], isect.texture_coord[1]);
}
