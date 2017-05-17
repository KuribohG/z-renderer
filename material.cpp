#include "material.h"

Material::Material() {}

void Material::set_color(const Vec3 &source) {
    color = source;
}

void Material::set_ambient(Float source) {
    ambient = source;
}

void Material::set_diffuse(Float source) {
    diffuse = source;
}

void Material::set_specular(Float source) {
    specular = source;
}

void Material::set_shininess(Float source) {
    shininess = source;
}

Vec3 Material::get_color() const {
    return color;
}

Float Material::get_ambient() const {
    return ambient;
}

Float Material::get_diffuse() const {
    return diffuse;
}

Float Material::get_specular() const {
    return specular;
}

Float Material::get_shininess() const {
    return shininess;
}
