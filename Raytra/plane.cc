//
//  plane.cc
//  Raytra
//
//  Created by Eagle Sky on 18/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#include "plane.h"

bool Plane::isIntersectedBy(const ray &r,  double &t) const
{
    point p0(0, _scalar, 0);
    double ln = r.getDir().dotProduct(_normal);
    if (ln < EPS && ln > -EPS) return false;
    double lk = _normal.dotProduct(p0 - r.getPoint());
    t = lk / ln;
    return t > 0;
}

bool Plane::intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const
{
    point p0(0, _scalar, 0);
    double ln = r.getDir().dotProduct(_normal);
    if (ln < EPS && ln > -EPS) return false;
    double lk = _normal.dotProduct(p0 - r.getPoint());
    t = lk / ln;
    if (t <= 0) return false;
    its = r.getPoint() + r.getDir() * t;
    normal = _normal;
    shading_normal = normal;
    return true;
}
