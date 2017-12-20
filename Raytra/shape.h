//
//  shape.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef shape_h
#define shape_h

#include "basicmath.h"
#include "material.h"
#include <array>

class Shape
{
public:
    Shape() {}
    Shape(const int mat_id) : _mat_id(mat_id) {}
    virtual bool isIntersectedBy(const ray &r,  double &t) const {return false;}
    virtual bool intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const {return false;}
    bool hitBBox(const ray &r) const;
    
    int getMaterialID() const {return _mat_id;}
    void setMaterial(int mat_id) {_mat_id = mat_id;}
    
    std::array<point, 2> getBoundingBox() const {return {_bbox_min, _bbox_max};}
    void setBoundingBox(const point &bbox_min, const point &bbox_max)
    {
        _bbox_min = bbox_min;
        _bbox_max = bbox_max;
    }
private:
    //TODO: Try to use an index instead of the object
    int _mat_id;
    point _bbox_min;
    point _bbox_max;
};

#endif /* shape_h */
