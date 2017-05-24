#ifndef RENDER_KDTREE_H
#define RENDER_KDTREE_H

#include <vector>

#include "bbox.h"
#include "shape.h"
#include "object.h"

class KdTreeNode {
public:
    BBox bbox;
    KdTreeNode *left;
    KdTreeNode *right;
    std::vector<Object *> objs;
    KdTreeNode();
    KdTreeNode *build(const std::vector<Object *> &objs, int depth);
};

class KdTree {
public:
    KdTreeNode *root;
    KdTree();
    KdTree(KdTreeNode *root);
    void build(const std::vector<Object *> &objs);
};

#endif //RENDER_KDTREE_H
