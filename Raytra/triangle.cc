//
//  triangle.cc
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#include "triangle.h"
#include "config.h"

bool Triangle::isIntersectedBy(const ray &r, double &t) const
{
    if (Config::getInstance().bboxesEnabled() && !hitBBox(r))
        return false;
    const point &A = points[0];
    const point &B = points[1];
    const point &C = points[2];
    
    const vec D = r.getDir();
    const point E = r.getPoint();
    
    double a = A[0] - B[0];
    double b = A[1] - B[1];
    double c = A[2] - B[2];
    
    double d = A[0] - C[0];
    double e = A[1] - C[1];
    double f = A[2] - C[2];
    
    double g = D[0];
    double h = D[1];
    double i = D[2];
    
    double j = A[0] - E[0];
    double k = A[1] - E[1];
    double l = A[2] - E[2];
    
    // reused composite values:
    double ei_hf = e * i - h * f;
    double gf_di = g * f - d * i;
    double dh_eg = d * h - e * g;
    double ak_jb = a * k - j * b;
    double jc_al = j * c - a * l;
    double bl_kc = b * l - k * c;
    
    // compute M:
    double M = a * ei_hf + b * gf_di + c * dh_eg;
    
    // compute t:
    t = -1. * (f * ak_jb + e * jc_al + d * bl_kc) / M;
    
    if (t <= 0.)
        return false;
    double alpha = (i * ak_jb + h * jc_al + g * bl_kc) / M;
    if ((alpha < 0.) || (alpha > 1.))
        return false;
    double beta = (j * ei_hf + k * gf_di + l * dh_eg) / M;
    if ((beta < 0.) || (beta > (1. - alpha)))
        return false;
    return true;
}

bool Triangle::intersect(const ray &r, point &its, vec &normal, vec &shading_normal, double &t) const
{
    if (Config::getInstance().bboxesEnabled() && !hitBBox(r))
        return false;
    const point &A = points[0];
    const point &B = points[1];
    const point &C = points[2];
    
    const vec D = r.getDir();
    const point E = r.getPoint();
    
    double a = A[0] - B[0];
    double b = A[1] - B[1];
    double c = A[2] - B[2];
    
    double d = A[0] - C[0];
    double e = A[1] - C[1];
    double f = A[2] - C[2];
    
    double g = D[0];
    double h = D[1];
    double i = D[2];
    
    double j = A[0] - E[0];
    double k = A[1] - E[1];
    double l = A[2] - E[2];
    
    // reused composite values:
    double ei_hf = e * i - h * f;
    double gf_di = g * f - d * i;
    double dh_eg = d * h - e * g;
    double ak_jb = a * k - j * b;
    double jc_al = j * c - a * l;
    double bl_kc = b * l - k * c;
    
    // compute M:
    double M = a * ei_hf + b * gf_di + c * dh_eg;
    
    // compute t:
    t = -1. * (f * ak_jb + e * jc_al + d * bl_kc) / M;
    
    if (t <= 0.)
        return false;
    double alpha = (i * ak_jb + h * jc_al + g * bl_kc) / M;
    if ((alpha < 0.) || (alpha > 1.))
        return false;
    double beta = (j * ei_hf + k * gf_di + l * dh_eg) / M;
    if ((beta < 0.) || (beta > (1. - alpha)))
        return false;
    its = E + D * t;
    normal = _normal;
    shading_normal = vert_normal[2] * alpha + vert_normal[1] * beta + vert_normal[0] * (1 - alpha - beta);
    shading_normal.normalize();
    return true;
}
