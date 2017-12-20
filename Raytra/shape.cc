//
//  shape.cc
//  Raytra
//
//  Created by Eagle Sky on 23/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#include "shape.h"

bool Shape::hitBBox(const ray &r) const
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
    
    return t_zmax >= t_xymin && t_xymax >= t_zmin;
}
