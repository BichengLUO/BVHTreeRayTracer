//
//  bvhtree.h
//  Raytra
//
//  Created by Eagle Sky on 23/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#ifndef bvhtree_h
#define bvhtree_h

#include <array>
#include <vector>
#include "basicmath.h"
#include "shape.h"

class BVHNode
{
public:
    BVHNode(): left(nullptr), right(nullptr), shape(nullptr) {}
    
    std::array<point, 2> getBoundingBox() const {return {_bbox_min, _bbox_max};}
    void setBoundingBox(const point &bbox_min, const point &bbox_max)
    {
        _bbox_min = bbox_min;
        _bbox_max = bbox_max;
    }
    bool hit(const ray &r, double &t) const;
    BVHNode *left;
    BVHNode *right;
    Shape *shape;
    
private:
    point _bbox_min;
    point _bbox_max;
};

class BVHTree
{
public:
    BVHTree(std::vector<Shape*> &objs);
    Shape* hit(const ray &r) const;
private:
    BVHNode *_root;
    BVHNode *constructBVHTree(const std::vector<Shape*> objs, int l, int r);
    bool hitNode(const ray &r, const BVHNode *root, double &t, Shape **shape) const;
};

#endif /* bvhtree_h */
