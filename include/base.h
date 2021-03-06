#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include <cstdio>
#include <vector>

typedef float Float;

const Float eps = 3e-3;

class Vec3 {
private:
    Float x, y, z;
public:
    Vec3();
    Vec3(Float x, Float y, Float z);
    Float get_x() const;
    Float get_y() const;
    Float get_z() const;
    Float length_sqr() const;
    Float length() const;
    Vec3 normalize() const;
    void set(int pos, Float x);
    Float operator[](int id) const;
    Vec3 operator-() const;
    Vec3& operator+=(const Vec3 &p);
    friend Vec3 operator+(const Vec3 &p, const Vec3 &q);
    friend Vec3 operator-(const Vec3 &p, const Vec3 &q);
    friend Vec3 operator*(const Vec3 &p, Float q);
    friend Vec3 operator*(Float p, const Vec3 &q);
    friend Vec3 operator/(const Vec3 &p, Float q);
    friend Float dot(const Vec3 &p, const Vec3 &q);
    friend Vec3 cross(const Vec3 &p, const Vec3 &q);
    friend Float distance_sqr(const Vec3 &p, const Vec3 &q);
};

Vec3 operator+(const Vec3 &p, const Vec3 &q);
Vec3 operator-(const Vec3 &p, const Vec3 &q);
Vec3 operator*(const Vec3 &p, Float q);
Vec3 operator*(Float p, const Vec3 &q);
Vec3 operator/(const Vec3 &p, Float q);
Float dot(const Vec3 &p, const Vec3 &q);
Vec3 cross(const Vec3 &p, const Vec3 &q);
Float distance_sqr(const Vec3 &p, const Vec3 &q);

template <typename T>
class Array2D {
private:
    std::vector<T> data;
public:
    int shape[2];
    Array2D() {}
    Array2D(int h, int w) {
        shape[0] = h;
        shape[1] = w;
        data.resize(h * w);
    }
    void init(int h, int w) {
        shape[0] = h;
        shape[1] = w;
        data.resize(h * w);
    }
    T get(int i, int j) const {
        return data[i * shape[1] + j];
    }
    void set(int i, int j, T x) {
        data[i * shape[1] + j] = x;
    }
};

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
