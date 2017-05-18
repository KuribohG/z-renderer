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
        return Vec3(0.0, 0.0, 0.0);
    }
    const Float INF = 1e20;
    Float dis = INF;
    Object *nearest = nullptr;
    Vec3 point;
    for (Object *obj : objs) {
        bool has_intersection = obj->get_shape()->intersect(r);
        if (has_intersection) {
            Vec3 intersection = obj->get_shape()->intersect_point(r);
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
        bool ok = true;
        for (Object *obj : objs) {
            bool has_intersection = obj->get_shape()->intersect(l);
            if (has_intersection) {
                Vec3 intersection = obj->get_shape()->intersect_point(l);
                Float d = distance_sqr(l.get_origin(), intersection);
                if (d < dis_light - eps) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            Vec3 normal = ((Sphere *)nearest->get_shape())->get_normal(point);
            normal = normal.normalize();
            Vec3 light_direction = ((PointLight *)light)->get_position() - point;
            light_direction = light_direction.normalize();
            Float diffuse = std::max(dot(normal, light_direction), (Float)0.0);
            color += nearest->get_material()->get_diffuse() *
                     ((PointLight *)light)->get_intensity() *
                     nearest->get_material()->get_color() * diffuse;
            //TODO: specular and shininess
        }
    }
    return color;
}
