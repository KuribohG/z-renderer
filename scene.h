#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>

#include "object.h"
#include "light.h"

class Scene {
private:
    std::vector<Object *> objs;
    std::vector<Light *> lights;
public:
    Scene();
    void add_obj(Object *obj);
    void add_light(Light *light);
    Vec3 li(const Ray &r, int depth);
};

#endif //RENDER_SCENE_H
