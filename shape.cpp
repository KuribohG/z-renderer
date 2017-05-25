#include <cmath>
#include <limits>
#include <algorithm>

#include "shape.h"

Sphere::Sphere(const Vec3 &position, Float radius)
        : position(position), radius(radius) {}

bool Sphere::intersect(const Ray &r) const {
    Float dx = r.get_direction().get_x();
    Float dy = r.get_direction().get_y();
    Float dz = r.get_direction().get_z();
    Float x0 = r.get_origin().get_x();
    Float y0 = r.get_origin().get_y();
    Float z0 = r.get_origin().get_z();
    Float cx = position.get_x();
    Float cy = position.get_y();
    Float cz = position.get_z();
    Float a = dx * dx + dy * dy + dz * dz;
    Float b = 2 * (dx * (x0 - cx) + dy * (y0 - cy) + dz * (z0 - cz));
    Float c = cx * cx + cy * cy + cz * cz + x0 * x0 +
            y0 * y0 + z0 * z0 - 2 * (cx * x0 + cy * y0 + cz * z0) - radius * radius;
    Float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    }
    Float t2 = (-b + std::sqrt(discriminant)) / (2 * a);
    return t2 > 0;
}

void Sphere::intersect_point(const Ray &r, Intersection &isect) const {
    Float dx = r.get_direction().get_x();
    Float dy = r.get_direction().get_y();
    Float dz = r.get_direction().get_z();
    Float x0 = r.get_origin().get_x();
    Float y0 = r.get_origin().get_y();
    Float z0 = r.get_origin().get_z();
    Float cx = position.get_x();
    Float cy = position.get_y();
    Float cz = position.get_z();
    Float a = dx * dx + dy * dy + dz * dz;
    Float b = 2 * (dx * (x0 - cx) + dy * (y0 - cy) + dz * (z0 - cz));
    Float c = cx * cx + cy * cy + cz * cz + x0 * x0 +
              y0 * y0 + z0 * z0 - 2 * (cx * x0 + cy * y0 + cz * z0) - radius * radius;
    Float discriminant = b * b - 4 * a * c;
    Float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    if (t1 > 0) {
        Vec3 point = r.get_origin() + t1 * r.get_direction();
        if (distance_sqr(r.get_origin(), position) < radius * radius) {
            isect.set_normal(position - point);
        } else {
            isect.set_normal(point - position);
        }
        isect.point = point;
    } else {
        Float t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        Vec3 point = r.get_origin() + t2 * r.get_direction();
        if (distance_sqr(r.get_origin(), position) < radius * radius) {
            isect.set_normal(position - point);
        } else {
            isect.set_normal(point - position);
        }
        isect.point = point;
    }
}

BBox Sphere::get_bbox() const {
    Vec3 min_v(position[0] - radius, position[1] - radius, position[2] - radius);
    Vec3 max_v(position[0] + radius, position[1] + radius, position[2] + radius);
    return BBox(min_v, max_v);
}

Triangle::Triangle(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2) {
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
}

bool Triangle::intersect(const Ray &r) const {
    Vec3 p = r.get_origin(), d = r.get_direction();
    Vec3 v0 = this->p[0], v1 = this->p[1], v2 = this->p[2];
    Vec3 e1 = v1 - v0, e2 = v2 - v0;
    Vec3 h = cross(d, e2);
    Float a = dot(e1, h);
    if (fabs(a) < eps) return false;
    Float f = 1 / a;
    Vec3 s = p - v0;
    Float u = f * dot(s, h);
    if (u < -eps || u > 1.0 + eps) return false;
    Vec3 q = cross(s, e1);
    Float v = f * dot(d, q);
    if (v < -eps || u + v > 1.0 + eps) return false;
    Float t = f * dot(e2, q);
    if (t > -eps) return true;
    return false;
}

void Triangle::intersect_point(const Ray &r, Intersection &isect) const {
    Vec3 p = r.get_origin(), d = r.get_direction();
    Vec3 v0 = this->p[0], v1 = this->p[1], v2 = this->p[2];
    Vec3 e1 = v1 - v0, e2 = v2 - v0;
    Vec3 h = cross(d, e2);
    Float a = dot(e1, h);
    Float f = 1 / a;
    Vec3 s = p - v0;
    Vec3 q = cross(s, e1);
    Float t = f * dot(e2, q);
    isect.point = p + t * d;
    Vec3 normal = cross(e1, e2);
    Float volume = dot(normal, p - v0);
    if (volume > 0) isect.set_normal(normal);
    else isect.set_normal(-normal);
}

BBox Triangle::get_bbox() const {
    Vec3 min_v, max_v;
    for (int i = 0; i < 3; i++) {
        Float min_x = std::numeric_limits<Float>::max();
        Float max_x = std::numeric_limits<Float>::lowest();
        for (int j = 0; j < 3; j++) {
            max_x = std::max(max_x, p[j][i]);
            min_x = std::min(min_x, p[j][i]);
        }
        min_v.set(i, min_x), max_v.set(i, max_x);
    }
    return BBox(min_v, max_v);
}