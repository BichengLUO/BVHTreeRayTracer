//
//  triangle.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef triangle_h
#define triangle_h

#include "basicmath.h"
#include "shape.h"

class Triangle : public Shape
{
public:
    Triangle() {}
    Triangle(point a, point b, point c, int mat_id): Shape(mat_id)
    {
        points[0] = a; points[1] = b; points[2] = c;
        _normal.crossProduct(b - a, c - a);
        _normal.normalize();
        vert_normal[0] = vert_normal[1] = vert_normal[2] = _normal;
        
        double min_x = std::min(a[0], std::min(b[0], c[0]));
        double min_y = std::min(a[1], std::min(b[1], c[1]));
        double min_z = std::min(a[2], std::min(b[2], c[2]));
        double max_x = std::max(a[0], std::max(b[0], c[0]));
        double max_y = std::max(a[1], std::max(b[1], c[1]));
        double max_z = std::max(a[2], std::max(b[2], c[2]));
        setBoundingBox(point(min_x, min_y, min_z),
                       point(max_x, max_y, max_z));
    }
    virtual bool isIntersectedBy(const ray &r,  double &t) const override;
    virtual bool intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const override;
    point points[3];
    vec vert_normal[3];
    
    vec getNormal() const {return _normal;}
private:
    vec _normal;
};

#endif /* triangle_h */
