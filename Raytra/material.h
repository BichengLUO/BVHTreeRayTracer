//
//  material.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef material_h
#define material_h

#include <iostream>

class Material
{
public:
    Material(){}
    Material(float dr, float dg, float db, float sr, float sg, float sb, double r, float ir, float ig, float ib): _phong_exp(r)
    {
        diffuse[0] = dr; diffuse[1] = dg; diffuse[2] = db;
        specular[0] = sr; specular[1] = sg; specular[2] = sb;
        ideal_spec[0] = ir; ideal_spec[1] = ig; ideal_spec[2] = ib;
    }
    float diffuse[3];
    float specular[3];
    float ideal_spec[3];
    
    double getPhongExp() const {return _phong_exp;}
private:
    double _phong_exp;
};

#endif /* material_h */
