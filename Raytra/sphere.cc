//
//  sphere.cc
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 24/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#include "sphere.h"
#include "config.h"

void Sphere::setPos(point pos)
{
    _pos = pos;
}

void Sphere::setPos(double x, double y, double z)
{
    _pos.set(x, y, z);
}

void Sphere::setRaidus(double r)
{
    _r = r;
}

bool Sphere::isIntersectedBy(const ray &r, double &t) const
{
    if (Config::getInstance().bboxesEnabled() && !hitBBox(r))
        return false;
    point pt = r.getPoint();
    vec dir = r.getDir();
    
    vec rpos_spos = pt - _pos;
    double dp = dir.dotProduct(rpos_spos);
    double dd = dir.dotProduct(dir);
    double pp = rpos_spos.dotProduct(rpos_spos);
    double delta = dp * dp - dd * (pp - _r * _r);
    if (delta < 0) return false;
    double t1 = (-dp + sqrt(delta)) / dd;
    double t2 = (-dp - sqrt(delta)) / dd;
    if (t1 <= 0 && t2 <= 0) return false;
    if (t1 <= 0 || t2 <= 0) t = std::max(t1, t2);
    else t = std::min(t1, t2);
    return true;
}

bool Sphere::intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const
{
    if (Config::getInstance().bboxesEnabled() && !hitBBox(r))
        return false;
    point pt = r.getPoint();
    vec dir = r.getDir();
    
    vec rpos_spos = pt - _pos;
    double dp = dir.dotProduct(rpos_spos);
    double dd = dir.dotProduct(dir);
    double pp = rpos_spos.dotProduct(rpos_spos);
    double delta = dp * dp - dd * (pp - _r * _r);
    if (delta < 0) return false;
    double t1 = (-dp + sqrt(delta)) / dd;
    double t2 = (-dp - sqrt(delta)) / dd;
    if (t1 <= 0 && t2 <= 0) return false;
    if (t1 <= 0 || t2 <= 0) t = std::max(t1, t2);
    else t = std::min(t1, t2);
    its = pt + dir * t;
    normal = its - _pos;
    normal.normalize();
    shading_normal = normal;
    return true;
}
