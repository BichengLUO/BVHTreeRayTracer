//
//  area_light.hpp
//  Raytra
//
//  Created by Eagle Sky on 05/11/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#ifndef area_light_h
#define area_light_h

#include "light.h"

class AreaLight : public Light
{
public:
    AreaLight(point pos, vec dir, vec u_dir, double len, float r, float g, float b): Light('s', pos, r, g, b), _dir(dir), _u_dir(u_dir), _len(len)
    {
        _v_dir.crossProduct(_dir, _u_dir);
        _dir.normalize();
        _u_dir.normalize();
        _v_dir.normalize();
    }
    
    vec getDir() const {return _dir;}
    vec getUDir() const {return _u_dir;}
    vec getVDir() const {return _v_dir;}
    double getLen() const {return _len;}
private:
    vec _dir;
    vec _u_dir;
    vec _v_dir;
    double _len;
};

#endif /* area_light_h */
