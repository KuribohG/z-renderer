#include "kdtree.h"

KdTreeNode::KdTreeNode() {
    bbox = BBox();
    left = nullptr;
    right = nullptr;
}

KdTreeNode *KdTreeNode::build(const std::vector<Object *> &objs, int depth) {
    this->objs = objs;
    this->left = nullptr;
    this->right = nullptr;
    this->bbox = BBox();



    return this;
}

KdTree::KdTree() {
    root = nullptr;
}

KdTree::KdTree(KdTreeNode *root)
        : root(root) {}

void KdTree::build(const std::vector<Object *> &objs) {
    root->build(objs, 0);
}
