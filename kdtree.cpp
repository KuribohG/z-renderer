#include <limits>

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

Object *KdTreeNode::hit(const Ray &r, Intersection &isect) const {
    Vec3 origin = r.get_origin();
    if (bbox.hit(r)) {
        if (left || right) {
            Object *hitleft = nullptr, *hitright = nullptr;
            Intersection isect1, isect2;
            if (left) hitleft = left->hit(r, isect1);
            if (right) hitright = right->hit(r, isect2);
            if (!hitleft && !hitright) return nullptr;
            Float d1 = std::numeric_limits<Float>::max(), d2 = std::numeric_limits<Float>::max();
            if (hitleft) d1 = distance_sqr(origin, isect1.point);
            if (hitright) d2 = distance_sqr(origin, isect2.point);
            if (d1 < d2) {
                isect = isect1;
                return hitleft;
            } else {
                isect = isect2;
                return hitright;
            }
        } else {
            Intersection isect_res;
            Object *intersect = nullptr;
            Float dis = std::numeric_limits<Float>::max();
            for (int i = 0; i < objs.size(); i++) {
                Shape *shape = objs[i]->get_shape();
                if (shape->intersect(r)) {
                    Intersection isect0;
                    shape->intersect_point(r, isect0);
                    Float d = distance_sqr(origin, isect0.point);
                    if (d < dis) {
                        dis = d;
                        objs[i]->intersect_point(r, isect_res);
                        intersect = objs[i];
                    }
                }
            }
            isect = isect_res;
            return intersect;
        }
    }
    return nullptr;
}

KdTree::KdTree()
        : root(nullptr) {}

KdTree::KdTree(KdTreeNode *root)
        : root(root) {}

void KdTree::build(const std::vector<Object *> &objs) {
    root = new KdTreeNode();
    root->build(objs, 0);
}

Object *KdTree::hit(const Ray &r, Intersection &isect) const {
    return root->hit(r, isect);
}
