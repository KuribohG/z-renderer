#include <cmath>

#include "shape.h"

Sphere::Sphere(Vec3 position, Float radius)
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
        isect.set_point(point);
    } else {
        Float t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        Vec3 point = r.get_origin() + t2 * r.get_direction();
        if (distance_sqr(r.get_origin(), position) < radius * radius) {
            isect.set_normal(position - point);
        } else {
            isect.set_normal(point - position);
        }
        isect.set_point(point);
    }
}