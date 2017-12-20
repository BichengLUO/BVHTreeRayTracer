//
//  camera.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 24/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef camera_h
#define camera_h

#include "basicmath.h"

class Camera
{
public:
    Camera() {}
    void init(point pos, vec dir,
              double d, double iw, double ih,
              int pw, int ph);
    void init(point pos, vec dir, vec up, vec right,
              double d, double iw, double ih,
              int pw, int ph);
    
    point getEyePos() const;
    vec getDir() const;
    vec getUp() const;
    vec getRight() const;
    double getFocalLen() const;
    void getImageSize(double &iw, double &ih) const;
    void getPixelSize(int &pw, int &ph) const;
    
private:
    point _eye;
    vec _dir;
    vec _up;
    vec _right;
    double _focal_len, _iw, _ih;
    int _pw, _ph;
};

#endif /* camera_h */
