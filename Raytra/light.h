//
//  light.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include "basicmath.h"
#include <ImfRgba.h>

class Light
{
public:
    Light(): Light('p', point(0, 0, 0), 0, 0, 0) {}
    Light(char tp, point pos, float r, float g, float b)
    {
        _type = tp;
        _pos = pos;
        color[0] = r; color[1] = g; color[2] = b;
    }
    
    point getPos() const {return _pos;}
    char getType() const {return _type;}
    float color[3];
private:
    char _type;
    point _pos;
};

#endif /* Light_h */
