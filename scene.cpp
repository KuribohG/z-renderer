#include <cmath>
#include <limits>

#include "scene.h"

Scene::Scene() {}

void Scene::add_obj(Object *obj) {
    objs.push_back(obj);
}

void Scene::add_light(Light *light) {
    lights.push_back(light);
}
