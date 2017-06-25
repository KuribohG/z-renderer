#ifndef RENDER_INTEGRATOR_H
#define RENDER_INTEGRATOR_H

#include "kdtree.h"
#include "camera.h"
#include "scene.h"

class WhittedIntegrator {
public:
    WhittedIntegrator();
    Vec3 li(const Ray &r, Scene *scene, int depth, KdTree *tree = nullptr);
    void render(unsigned char *dst, int height, int width, Camera *camera, Scene *scene, int depth, KdTree *tree = nullptr);
};


#endif //RENDER_INTEGRATOR_H
