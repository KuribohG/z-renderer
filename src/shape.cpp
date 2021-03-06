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

Vec3 Sphere::surface_normal(const Vec3 &p) const {
    return (p - position).normalize();
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

Vec3 Triangle::surface_normal(const Vec3 &x) const {
    return cross(p[1] - p[0], p[2] - p[0]).normalize();
}

TriangleMesh::TriangleMesh(int n_vertices, int n_vertice_textures, int n_vertice_normals, int n_triangles)
        : n_vertices(n_vertices), n_triangles(n_triangles) {
    v = new Vec3[n_vertices];
    vt = new Vec3[n_vertice_textures];
    vn = new Vec3[n_vertice_normals];
    faces_v = new int[n_triangles * 3];
    faces_vt = new int[n_triangles * 3];
    faces_vn = new int[n_triangles * 3];
}

TriangleMesh::~TriangleMesh() {
    delete [] v;
    delete [] vt;
    delete [] vn;
    delete [] faces_v;
    delete [] faces_vt;
    delete [] faces_vn;
}

MeshTriangle::MeshTriangle(TriangleMesh *mesh, int id)
        : mesh(mesh) {
    start_v = &(mesh->faces_v[id * 3]);
    start_vt = &(mesh->faces_vt[id * 3]);
    start_vn = &(mesh->faces_vn[id * 3]);
    //TODO: check validity
    if ((*start_vt) >= 0) state |= (1 << HAS_TEXTURE);
    if ((*start_vn) >= 0) state |= (1 << HAS_NORMAL);
}

bool MeshTriangle::check_attribute(MeshTriangleAttribute attr) const {
    return bool(state & (1 << attr));
}

bool MeshTriangle::intersect(const Ray &r) const {
    Vec3 p = r.get_origin(), d = r.get_direction();
    Vec3 v0 = mesh->v[start_v[0]], v1 = mesh->v[start_v[1]], v2 = mesh->v[start_v[2]];
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

void MeshTriangle::intersect_point(const Ray &r, Intersection &isect) const {
    Vec3 p = r.get_origin(), d = r.get_direction();
    Vec3 v0 = mesh->v[start_v[0]], v1 = mesh->v[start_v[1]], v2 = mesh->v[start_v[2]];
    Vec3 e1 = v1 - v0, e2 = v2 - v0;
    Vec3 h = cross(d, e2);
    Float a = dot(e1, h);
    Float f = 1 / a;
    Vec3 s = p - v0;
    Vec3 q = cross(s, e1);
    Float t = f * dot(e2, q);
    isect.point = p + t * d;
    Vec3 N = cross(e1, e2);
    Float denom = dot(N, N);
    Float x = dot(N, cross(v2 - v1, isect.point - v1)) / denom;
    Float y = dot(N, cross(v0 - v2, isect.point - v2)) / denom;
    isect.barycentric = Vec3(x, y, 1.0 - x - y);
    Float u = isect.barycentric[0], v = isect.barycentric[1];
    if (check_attribute(HAS_NORMAL)) {
        Vec3 normal = u * mesh->vn[start_vn[0]] + v * mesh->vn[start_vn[1]] + (1.0 - u - v) * mesh->vn[start_vn[2]];
        isect.set_normal(normal);
    } else {
        Vec3 normal = cross(e1, e2);
        Float volume = dot(normal, p - v0);
        if (volume > 0) isect.set_normal(normal);
        else isect.set_normal(-normal);
    }
    if (check_attribute(HAS_TEXTURE)) {
        isect.texture_coord = u * mesh->vt[start_vt[0]] + v * mesh->vt[start_vt[1]] + (1.0 - u - v) * mesh->vt[start_vt[2]];
    }
}

BBox MeshTriangle::get_bbox() const {
    Vec3 min_v, max_v;
    Vec3 p[3] = {mesh->v[start_v[0]], mesh->v[start_v[1]], mesh->v[start_v[2]]};
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

Vec3 MeshTriangle::surface_normal(const Vec3 &p) const {
    Vec3 v0 = mesh->v[start_v[0]], v1 = mesh->v[start_v[1]], v2 = mesh->v[start_v[2]];
    if (check_attribute(HAS_NORMAL)) {
        Vec3 e1 = v1 - v0, e2 = v2 - v0;
        Vec3 N = cross(e1, e2);
        Float denom = dot(N, N);
        Float x = dot(N, cross(v2 - v1, p - v1)) / denom;
        Float y = dot(N, cross(v0 - v2, p - v2)) / denom;
        Vec3 normal = x * mesh->vn[start_vn[0]] + y * mesh->vn[start_vn[1]] + (1.0 - x - y) * mesh->vn[start_vn[2]];
        return normal;
    } else {
        return cross(v1 - v0, v2 - v0).normalize();
    }
}