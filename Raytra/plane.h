//
//  plane.h
//  Raytra
//
//  Created by Eagle Sky on 18/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#ifndef plane_h
#define plane_h

#include "basicmath.h"
#include "shape.h"

class Plane : public Shape
{
public:
    Plane() {}
    Plane(vec normal, double scalar, int mat_id): _scalar(scalar), Shape(mat_id)
    {
        _normal = normal;
        _normal.normalize();
    }
    virtual bool isIntersectedBy(const ray &r,  double &t) const override;
    virtual bool intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const override;
    
    vec getNormal() const {return _normal;}
    double getScalar() const {return _scalar;}
private:
    vec _normal;
    double _scalar;
};

#endif /* plane_h */
