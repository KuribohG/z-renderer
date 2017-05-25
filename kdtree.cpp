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

    int axis = depth % 3;

    std::vector<Float> mid;
    for (Object *obj : objs) {
        BBox x = obj->get_shape()->get_bbox();
        bbox.expand(x);
        Float t = x.midpoint()[axis];
        mid.push_back(t);
    }
    std::nth_element(mid.begin(), mid.begin() + mid.size() / 2, mid.end());
    Float median = mid[mid.size() / 2];

    std::vector<Object *> left_objs, right_objs;
    for (int i = 0; i < objs.size(); i++) {
        BBox x = objs[i]->get_shape()->get_bbox();
        Float t = x.midpoint()[axis];
        if (t <= median) {
            left_objs.push_back(objs[i]);
        } else {
            right_objs.push_back(objs[i]);
        }
    }

    if (left_objs.size() > 0 && right_objs.size() > 0) {
        left = new KdTreeNode();
        left->build(left_objs, depth + 1);
        right = new KdTreeNode();
        right->build(right_objs, depth + 1);
    }

    return this;
}

KdTree::KdTree()
        : root(nullptr) {}

KdTree::KdTree(KdTreeNode *root)
        : root(root) {}

void KdTree::build(const std::vector<Object *> &objs) {
    root = new KdTreeNode();
    root->build(objs, 0);
}
