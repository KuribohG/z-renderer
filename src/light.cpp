#include "light.h"

PointLight::PointLight() {}

PointLight::PointLight(const Vec3 &position, Float intensity)
        : position(position), intensity(intensity) {}

Vec3 PointLight::get_position() const {
    return position;
}

Float PointLight::get_intensity() const {
    return intensity;
}
