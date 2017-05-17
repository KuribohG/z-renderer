#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include "base.h"

class Material {
private:
    Vec3 color;
    Float ambient;
    Float diffuse;
    Float specular;
    Float shininess;
public:
    Material();
    void set_color(const Vec3 &source);
    void set_ambient(Float source);
    void set_diffuse(Float source);
    void set_specular(Float source);
    void set_shininess(Float source);
    Vec3 get_color() const;
    Float get_ambient() const;
    Float get_diffuse() const;
    Float get_specular() const;
    Float get_shininess() const;
};

#endif //RENDER_MATERIAL_H
