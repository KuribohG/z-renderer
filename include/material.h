#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include "base.h"
#include "texture.h"
#include "intersection.h"

enum MaterialType {PHONG};

class Material {
public:
    MaterialType type;
    virtual void complete_intersection(Intersection &isect) const = 0;
};

class PhongMaterial : public Material {
public:
    Texture2D<Vec3> *texture;
    Float ambient;
    Float diffuse;
    Float specular;
    Float shininess;
    Float reflectivity;
    PhongMaterial();
    virtual ~PhongMaterial();
    void set_color(const Vec3 &color);
    virtual void complete_intersection(Intersection &isect) const;
};

#endif //RENDER_MATERIAL_H
