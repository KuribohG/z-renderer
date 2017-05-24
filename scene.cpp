#include <cmath>

#include "scene.h"

Scene::Scene() {}

void Scene::add_obj(Object *obj) {
    objs.push_back(obj);
}

void Scene::add_light(Light *light) {
    lights.push_back(light);
}

Vec3 Scene::li(const Ray &r, int depth) {
    Vec3 color(0.0, 0.0, 0.0);
    if (depth == 0) {
        return color;
    }
    const Float INF = 1e20;
    Float dis = INF;
    Object *nearest = nullptr;
    Vec3 point;
    for (Object *obj : objs) {
        bool has_intersection = obj->get_shape()->intersect(r);
        if (has_intersection) {
            Intersection isect;
            obj->get_shape()->intersect_point(r, isect);
            Vec3 intersection = isect.get_point();
            Float d = distance_sqr(r.get_origin(), intersection);
            if (d < dis) {
                dis = d;
                nearest = obj;
                point = intersection;
            }
        }
    }
    if (!nearest) {
        return color;
    }
    color += nearest->get_material()->get_ambient() * nearest->get_material()->get_color();
    for (Light *light : lights) {
        Vec3 origin = ((PointLight *)light)->get_position();
        Ray l(origin, point - origin);
        Float dis_light = distance_sqr(l.get_origin(), point);
        Intersection isect;
        bool ok = true;
        for (Object *obj : objs) {
            bool has_intersection = obj->get_shape()->intersect(l);
            if (has_intersection) {
                obj->get_shape()->intersect_point(l, isect);
                Vec3 intersection = isect.get_point();
                Float d = distance_sqr(l.get_origin(), intersection);
                if (d < dis_light - eps) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            Float intensity = ((PointLight *)light)->get_intensity();
            Vec3 normal = isect.get_normal();
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
            Float reflectivity = nearest->get_material()->get_reflectivity();
            if (reflectivity > 0.0) {
                Vec3 reflect_direction = -light_direction - 2.0 * dot(-light_direction, normal) * normal;
                reflect_direction = reflect_direction.normalize();
                color += reflectivity * li(Ray(point + 0.01 * reflect_direction, reflect_direction), depth - 1);
            }
        }
    }
    return color;
}
