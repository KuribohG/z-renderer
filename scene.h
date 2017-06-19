#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>

#include "object.h"
#include "light.h"
#include "kdtree.h"

class Scene {
public:
    std::vector<Object *> objs;
    std::vector<Light *> lights;
    Scene();
    void add_obj(Object *obj);
    void add_light(Light *light);
    Vec3 li(const Ray &r, int depth, KdTree *tree = nullptr, bool ambient = false);
};

#endif //RENDER_SCENE_H
