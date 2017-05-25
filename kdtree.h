#ifndef RENDER_KDTREE_H
#define RENDER_KDTREE_H

#include <vector>

#include "bbox.h"
#include "object.h"

class KdTreeNode {
public:
    BBox bbox;
    KdTreeNode *left;
    KdTreeNode *right;
    std::vector<Object *> objs;
    KdTreeNode();
    KdTreeNode *build(const std::vector<Object *> &objs, int depth);
    Object *hit(const Ray &r, Intersection &isect) const;
};

class KdTree {
public:
    KdTreeNode *root;
    KdTree();
    KdTree(KdTreeNode *root);
    void build(const std::vector<Object *> &objs);
    Object *hit(const Ray &r, Intersection &isect) const;
};

#endif //RENDER_KDTREE_H
