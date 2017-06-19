#include <cmath>
#include <algorithm>

#include "integrator.h"

#include "omp.h"

WhittedIntegrator::WhittedIntegrator() {}

Vec3 WhittedIntegrator::li(const Ray &r, Scene *scene, int depth, KdTree *tree) {
    Vec3 color(0.0, 0.0, 0.0);
    if (depth == 0) {
        return color;
    }
    Object *nearest = nullptr;
    Vec3 point, normal;
    if (!tree) {
        Float dis = std::numeric_limits<Float>::max();
        for (Object *obj : scene->objs) {
            bool has_intersection = obj->get_shape()->intersect(r);
            if (has_intersection) {
                Intersection isect;
                obj->get_shape()->intersect_point(r, isect);
                Vec3 intersection = isect.point;
                Float d = distance_sqr(r.get_origin(), intersection);
                if (d < dis) {
                    dis = d;
                    nearest = obj;
                    point = intersection;
                }
            }
        }
    } else {
        Intersection isect;
        nearest = tree->hit(r, isect);
        point = isect.point;
        normal = isect.normal;
    }
    if (!nearest) {
        return color;
    }
    color += nearest->get_material()->get_ambient() * nearest->get_material()->get_color();
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
            color += nearest->get_material()->get_diffuse() * intensity *
                     nearest->get_material()->get_color() * diffuse;
            Float shininess = nearest->get_material()->get_shininess();
            Float specular = nearest->get_material()->get_specular();
            Vec3 view_direction = r.get_origin() - point;
            view_direction = view_direction.normalize();
            Vec3 halfway_direction = (view_direction + light_direction).normalize();
            Float spec = std::pow(std::max(dot(normal, halfway_direction), (Float)0.0), shininess);
            Vec3 col_specular = spec * intensity * Vec3(1.0, 1.0, 1.0) * specular;
            color += col_specular;
        }
    }
    Float reflectivity = nearest->get_material()->get_reflectivity();
    if (reflectivity > 0.0) {
        Vec3 direction = r.get_direction();
        Vec3 reflect_direction = direction - 2.0 * dot(direction, normal) * normal;
        reflect_direction = reflect_direction.normalize();
        color += reflectivity * li(Ray(point + 0.5 * reflect_direction, reflect_direction), scene, depth - 1, tree);
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