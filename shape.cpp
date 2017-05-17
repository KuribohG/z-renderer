#include <cmath>

#include "shape.h"

Sphere::Sphere(Vec3 position, Float radius)
        : position(position), radius(radius) {}

bool Sphere::intersect(const Ray &r) {
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
    return discriminant >= 0;
}

Vec3 Sphere::intersect_point(const Ray &r) {
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
    Float t = (-b - std::sqrt(discriminant)) / (2 * a);
    return r.get_origin() + t * r.get_direction();
}