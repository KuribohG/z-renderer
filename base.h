#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include <cstdio>

typedef float Float;

class Vec3 {
private:
    Float x, y, z;
public:
    Vec3();
    Vec3(Float x, Float y, Float z);
    Float get_x() const;
    Float get_y() const;
    Float get_z() const;
    Float length() const;
    Vec3 normalize() const;
    Float operator[](int id) const;
    friend Vec3 operator+(const Vec3 &p, const Vec3 &q);
    friend Vec3 operator-(const Vec3 &p, const Vec3 &q);
    friend Vec3 operator*(const Vec3 &p, Float q);
    friend Vec3 operator*(Float p, const Vec3 &q);
    friend Vec3 operator/(const Vec3 &p, Float q);
    friend Float dot(const Vec3 &p, const Vec3 &q);
    friend Vec3 cross(const Vec3 &p, const Vec3 &q);
};

Vec3 operator+(const Vec3 &p, const Vec3 &q);
Vec3 operator-(const Vec3 &p, const Vec3 &q);
Vec3 operator*(const Vec3 &p, Float q);
Vec3 operator*(Float p, const Vec3 &q);
Vec3 operator/(const Vec3 &p, Float q);
Float dot(const Vec3 &p, const Vec3 &q);
Vec3 cross(const Vec3 &p, const Vec3 &q);

class Ray {
private:
    Vec3 origin;
    Vec3 direction;
public:
    Ray();
    Ray(const Vec3 &origin, const Vec3 &direction);
    Vec3 get_origin() const;
    Vec3 get_direction() const;
};

#endif //RENDER_BASE_H
