#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include "base.h"

class Light {};

class PointLight : public Light {
private:
    Vec3 position;
    Float intensity;
public:
    PointLight();
    PointLight(const Vec3 &position, Float intensity);
    Vec3 get_position() const;
    Float get_intensity() const;
};

#endif //RENDER_LIGHT_H
