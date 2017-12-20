//
//  camera.cc
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 24/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#include "camera.h"

void Camera::init(point pos, vec dir,
          double d, double iw, double ih,
          int pw, int ph)
{
    vec right;
    dir.normalize();
    if (1 - fabs(dir[1]) < EPS)
        right = vec(1, 0, 0);
    else
        right.crossProduct(dir, vec(0, 1, 0));
    vec up;
    up.crossProduct(right, dir);
    
    init(pos, dir, up, right, d, iw, ih, pw, ph);
}

void Camera::init(point pos, vec dir, vec up, vec right,
          double d, double iw, double ih,
          int pw, int ph)
{
    dir.normalize();
    up.normalize();
    right.normalize();
    
    _eye = pos;
    _dir = dir;
    _up = up;
    _right = right;
    
    _focal_len = d;
    _iw = iw;
    _ih = ih;
    
    _pw = pw;
    _ph = ph;
}

point Camera::getEyePos() const
{
    return _eye;
}

vec Camera::getDir() const
{
    return _dir;
}

vec Camera::getUp() const
{
    return _up;
}

vec Camera::getRight() const
{
    return _right;
}

double Camera::getFocalLen() const
{
    return _focal_len;
}

void Camera::getImageSize(double &iw, double &ih) const
{
    iw = _iw;
    ih = _ih;
}

void Camera::getPixelSize(int &pw, int &ph) const
{
    pw = _pw;
    ph = _ph;
}
