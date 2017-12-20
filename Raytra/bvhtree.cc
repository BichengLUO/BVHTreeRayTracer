//
//  bvhtree.cpp
//  Raytra
//
//  Created by Eagle Sky on 23/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#include "bvhtree.h"
#include <algorithm>

bool BVHNode::hit(const ray &r, double &t) const
{
    point eye = r.getPoint();
    vec d = r.getDir();
    double t_xmin = (_bbox_min[0] - eye[0]) / d[0];
    double t_xmax = (_bbox_max[0] - eye[0]) / d[0];
    if (t_xmin > t_xmax) std::swap(t_xmin, t_xmax);
    
    double t_ymin = (_bbox_min[1] - eye[1]) / d[1];
    double t_ymax = (_bbox_max[1] - eye[1]) / d[1];
    if (t_ymin > t_ymax) std::swap(t_ymin, t_ymax);
    
    if (t_xmax < t_ymin || t_ymax < t_xmin) return false;
    double t_xymin = std::max(t_xmin, t_ymin);
    double t_xymax = std::min(t_xmax, t_ymax);
    
    double t_zmin = (_bbox_min[2] - eye[2]) / d[2];
    double t_zmax = (_bbox_max[2] - eye[2]) / d[2];
    if (t_zmin > t_zmax) std::swap(t_zmin, t_zmax);
    
    bool hit = t_zmax >= t_xymin && t_xymax >= t_zmin;
    if (hit) t = std::max(t_xymin, t_zmin);
    return hit;
}

BVHTree::BVHTree(std::vector<Shape*> &objs)
{
    std::sort(objs.begin(), objs.end(), [](Shape *l, Shape *r){
        auto l_bbox = l->getBoundingBox();
        auto r_bbox = r->getBoundingBox();
        point lc((l_bbox[0][0] + l_bbox[1][0]) / 2.0,
                 (l_bbox[0][1] + l_bbox[1][1]) / 2.0,
                 (l_bbox[0][2] + l_bbox[1][2]) / 2.0);
        point rc((r_bbox[0][0] + r_bbox[1][0]) / 2.0,
                 (r_bbox[0][1] + r_bbox[1][1]) / 2.0,
                 (r_bbox[0][2] + r_bbox[1][2]) / 2.0);
        if (lc[0] == rc[0])
        {
            if (lc[1] == rc[1]) return lc[2] < rc[2];
            else return lc[1] < rc[1];
        }
        else return lc[0] < rc[0];
    });
    _root = constructBVHTree(objs, 0, objs.size() - 1);
}

Shape* BVHTree::hit(const ray &r) const
{
    Shape *shape = nullptr;
    double t;
    hitNode(r, _root, t, &shape);
    return shape;
}

BVHNode* BVHTree::constructBVHTree(const std::vector<Shape*> objs, int l, int r)
{
    if (l == r)
    {
        BVHNode *node = new BVHNode;
        node->shape = objs[l];
        auto bbox = objs[l]->getBoundingBox();
        node->setBoundingBox(bbox[0], bbox[1]);
        return node;
    }
    int m = (l + r) / 2;
    BVHNode *node = new BVHNode;
    node->left = constructBVHTree(objs, l, m);
    node->right = constructBVHTree(objs, m + 1, r);
    auto l_bbox = node->left->getBoundingBox();
    auto r_bbox = node->right->getBoundingBox();
    point bbox_min(std::min(l_bbox[0][0], r_bbox[0][0]),
                   std::min(l_bbox[0][1], r_bbox[0][1]),
                   std::min(l_bbox[0][2], r_bbox[0][2]));
    point bbox_max(std::max(l_bbox[1][0], r_bbox[1][0]),
                   std::max(l_bbox[1][1], r_bbox[1][1]),
                   std::max(l_bbox[1][2], r_bbox[1][2]));
    node->setBoundingBox(bbox_min, bbox_max);
    return node;
}

bool BVHTree::hitNode(const ray &r, const BVHNode *root, double &t, Shape **shape) const
{
    double t_root = std::numeric_limits<double>::max();
    if (!root->hit(r, t_root)) return false;
    if (!root->left && !root->right)
    {
        *shape = root->shape;
        return root->shape->isIntersectedBy(r, t) && t > EPS;
    }
    double tl = std::numeric_limits<double>::max(), tr = std::numeric_limits<double>::max();
    Shape *sh_l = nullptr, *sh_r = nullptr;
    bool l_hit = hitNode(r, root->left, tl, &sh_l);
    bool r_hit = hitNode(r, root->right, tr, &sh_r);
    if (!l_hit && !r_hit) return false;
    if (l_hit && r_hit)
    {
        t = std::min(tl, tr);
        if (tl < tr) *shape = sh_l;
        else *shape = sh_r;
    }
    else
    {
        if (l_hit)
        {
            *shape = sh_l;
            t = tl;
        }
        else
        {
            *shape = sh_r;
            t = tr;
        }
    }
    return true;
}
