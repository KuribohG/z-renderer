#include <cmath>
#include <cassert>

#include "base.h"

Vec3::Vec3() {}

Vec3::Vec3(Float x, Float y, Float z)
    : x(x), y(y), z(z) {}

Float Vec3::get_x() const {
    return x;
}

Float Vec3::get_y() const {
    return y;
}

Float Vec3::get_z() const {
    return z;
}

Float Vec3::length() const {
    return (Float)sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const {
    return (*this) / length();
}

Float Vec3::operator[](int id) const {
    assert(id >= 0 && id < 3);
    if (id == 0) {
        return x;
    } else if (id == 1) {
        return y;
    } else {
        return z;
    }
}

Vec3 operator+(const Vec3 &p, const Vec3 &q) {
    return Vec3(p.x + q.x, p.y + q.y, p.z + q.z);
}

Vec3 operator-(const Vec3 &p, const Vec3 &q) {
    return Vec3(p.x - q.x, p.y - q.y, p.z - q.z);
}

Vec3 operator*(const Vec3 &p, Float q) {
    return Vec3(p.x * q, p.y * q, p.z * q);
}

Vec3 operator*(Float p, const Vec3 &q) {
    return Vec3(p * q.x, p * q.y, p * q.z);
}

Vec3 operator/(const Vec3 &p, Float q) {
    return Vec3(p.x / q, p.y / q, p.z / q);
}

Float dot(const Vec3 &p, const Vec3 &q) {
    return p.x * q.x + p.y * q.y + p.z * q.z;
}

Vec3 cross(const Vec3 &p, const Vec3 &q) {
    return Vec3(p.y * q.z - p.z * q.y,
                p.z * q.x - p.x * q.z,
                p.x * q.y - p.y * q.x);
}

Ray::Ray() {}

Ray::Ray(const Vec3 &origin, const Vec3 &direction)
        : origin(origin), direction(direction.normalize()) {}

Vec3 Ray::get_origin() const {
    return origin;
}

Vec3 Ray::get_direction() const {
    return direction;
}
