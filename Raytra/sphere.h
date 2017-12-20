//
//  sphere.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 24/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef sphere_h
#define sphere_h

#include "basicmath.h"
#include "material.h"
#include "shape.h"

class Sphere : public Shape
{
public:
    Sphere() {}
    Sphere(point pos, double r, int mat_id): Shape(mat_id), _pos(pos), _r(r)
    {
        setBoundingBox(point(pos[0] - r, pos[1] - r, pos[2] - r),
                       point(pos[0] + r, pos[1] + r, pos[2] + r));
    }
    
    point getPos() const {return _pos;};
    void setPos(point pos);
    void setPos(double x, double y, double z);
    
    double getRadius() const {return _r;}
    void setRaidus(double r);
    virtual bool isIntersectedBy(const ray &r,  double &t) const override;
    virtual bool intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const override;
private:
    point _pos;
    double _r;
};

#endif /* sphere_h */
