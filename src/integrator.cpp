#include <cmath>
#include <algorithm>

#include "integrator.h"
#include "material.h"

#include "omp.h"

WhittedIntegrator::WhittedIntegrator() {}

Vec3 WhittedIntegrator::li(const Ray &r, Scene *scene, int depth, KdTree *tree) {
    Vec3 color(0.0, 0.0, 0.0);
    if (depth == 0) {
        return color;
    }
    Object *nearest = nullptr;
    Vec3 point, normal;
    Intersection obj_isect;
    if (!tree) {
        Float dis = std::numeric_limits<Float>::max();
        for (Object *obj : scene->objs) {
            bool has_intersection = obj->get_shape()->intersect(r);
            if (has_intersection) {
                Intersection isect;
                obj->intersect_point(r, isect);
                Vec3 intersection = isect.point;
                Float d = distance_sqr(r.get_origin(), intersection);
                if (d < dis) {
                    dis = d;
                    nearest = obj;
                    point = intersection;
                    obj_isect = isect;
                }
            }
        }
    } else {
        Intersection isect;
        nearest = tree->hit(r, isect);
        point = isect.point;
        normal = isect.normal;
        obj_isect = isect;
    }
    if (!nearest) {
        return color;
    }
    Material *mat = nearest->get_material();
    if (mat->type == PHONG) {
        Vec3 col = obj_isect.color;
        PhongMaterial *phong_mat = (PhongMaterial *)mat;
        color += phong_mat->ambient * col;
        for (Light *light : scene->lights) {
            Vec3 origin = ((PointLight *)light)->get_position();
            Ray l(origin, point - origin);
            Intersection isect;
            bool ok = true;
            Float dis_light = distance_sqr(origin, point);
            if (!tree) {
                for (Object *obj : scene->objs) {
                    bool has_intersection = obj->get_shape()->intersect(l);
                    if (has_intersection) {
                        obj->get_shape()->intersect_point(l, isect);
                        Vec3 intersection = isect.point;
                        Float d = distance_sqr(origin, intersection);
                        if (d < dis_light - eps) {
                            ok = false;
                            break;
                        }
                    }
                }
            } else {
                Object *obj = tree->hit(l, isect);
                if (obj) {
                    Float d = distance_sqr(origin, isect.point);
                    if (d < dis_light - eps) ok = false;
                }
            }
            Vec3 normal = isect.normal;
            if (ok) {
                Float intensity = ((PointLight *)light)->get_intensity();
                Vec3 light_direction = origin - point;
                light_direction = light_direction.normalize();
                Float diffuse = std::max(dot(normal, light_direction), (Float)0.0);
                color += phong_mat->diffuse * intensity * col * diffuse;
                Float shininess = phong_mat->shininess;
                Float specular = phong_mat->specular;
                Vec3 view_direction = r.get_origin() - point;
                view_direction = view_direction.normalize();
                Vec3 halfway_direction = (view_direction + light_direction).normalize();
                Float spec = std::pow(std::max(dot(normal, halfway_direction), (Float)0.0), shininess);
                Vec3 col_specular = spec * intensity * Vec3(1.0, 1.0, 1.0) * specular;
                color += col_specular;
            }
        }
        Float reflectivity = phong_mat->reflectivity;
        if (reflectivity > 0.0) {
            Vec3 direction = r.get_direction();
            Vec3 reflect_direction = direction - 2.0 * dot(direction, normal) * normal;
            reflect_direction = reflect_direction.normalize();
            color += reflectivity * li(Ray(point + 0.5 * reflect_direction, reflect_direction), scene, depth - 1, tree);
        }
        Float refraction = phong_mat->refraction;
        if (refraction > 0.0) {
            Vec3 direction = r.get_direction();
            direction = direction / abs(dot(direction, normal));
            Float kn = phong_mat->etaA / phong_mat->etaB;
            Vec3 surface_normal = nearest->get_shape()->surface_normal(point);
            if (dot(direction, surface_normal) > 0) kn = 1 / kn;
            Float kf = 1.0 / sqrt(kn * kn * direction.length_sqr() - (direction + normal).length_sqr());
            Vec3 refraction_direction = kf * (normal + direction) - normal;
            Ray refraction_ray(point + 0.1 * refraction_direction, refraction_direction);
            color += refraction * li(refraction_ray, scene, depth - 1, tree);
        }
        return color;
    }
    return color;
}

void WhittedIntegrator::render(unsigned char *dst, int height, int width, Camera *camera, Scene *scene, int depth, KdTree *tree) {
    #pragma omp parallel for schedule(dynamic, 1) collapse(2)
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            Ray r = camera->get_ray_through_pixel(i, j, width, height);
            Vec3 color = li(r, scene, depth, tree);
            int id = (height - 1 - j) * width * 3 + i * 3;
            dst[id++] = (unsigned char)(std::min(color[0], (Float)1.0) * 256);
            dst[id++] = (unsigned char)(std::min(color[1], (Float)1.0) * 256);
            dst[id++] = (unsigned char)(std::min(color[2], (Float)1.0) * 256);
        }
    }
}