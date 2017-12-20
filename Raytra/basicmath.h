//
//  basicmath.h
//  raytra_1
//

#ifndef BASICMATH_H_
#define BASICMATH_H_

#include <cassert>
#include <iostream>
#include <cmath>

#define EPS 1e-4

class vec
{
public:
    vec() {}
    vec(const vec& v)
    {
        ijk_[0] = v[0];
        ijk_[1] = v[1];
        ijk_[2] = v[2];
    }
    vec(double i, double j, double k)
    {
        ijk_[0] = i;
        ijk_[1] = j;
        ijk_[2] = k;
    }
    
    void set(double i, double j, double k)
    {
        ijk_[0] = i;
        ijk_[1] = j;
        ijk_[2] = k;
    }

    double operator[](const int i) const;
    
    void normalize();
    
    // return scaled version of this vector:
    vec operator*(const double scale) const
    {
        return vec(scale * ijk_[0],scale * ijk_[1],scale * ijk_[2]);
    }
    
    vec invert() const
    {
        // call the above scaling operator to invert:
        return operator* (-1.0); 
    }
    
    // allow vector addition via +=
    vec& operator+=(const vec &v)
    {
        ijk_[0] = ijk_[0] + v.ijk_[0];
        ijk_[1] = ijk_[1] + v.ijk_[1];
        ijk_[2] = ijk_[2] + v.ijk_[2];
        return *this;
    }
    
    vec operator+(const vec &v) const
    {
        return vec(ijk_[0] + v.ijk_[0],
                   ijk_[1] + v.ijk_[1],
                   ijk_[2] + v.ijk_[2]);
    }
    
    vec operator-(const vec &v) const
    {
        return vec(ijk_[0] - v.ijk_[0],
                   ijk_[1] - v.ijk_[1],
                   ijk_[2] - v.ijk_[2]);
    }
   
    // returns the scalar value that is the dotproduct
    // between this and v:
    double dotProduct(const vec &v) const
    {
        return (ijk_[0] * v.ijk_[0] + ijk_[1] * v.ijk_[1] + ijk_[2] * v.ijk_[2]);
    }
    
    double length() const
    {
        return sqrt(this->dotProduct(*this));
    }
    // sets this to be the cross product of vectors va and vb:
    // i.e. va X vb
    void crossProduct(const vec &va, const vec &vb)
    {
        ijk_[0] = va.ijk_[1] * vb.ijk_[2] - va.ijk_[2] * vb.ijk_[1];
        ijk_[1] = va.ijk_[2] * vb.ijk_[0] - va.ijk_[0] * vb.ijk_[2];
        ijk_[2] = va.ijk_[0] * vb.ijk_[1] - va.ijk_[1] * vb.ijk_[0];
    }
    
    // these two functions are used to allow us to read in
    // and print out (or save to a file). they are used by
    // the parser code.
    friend std::istream &operator>>(std::istream &is, vec &v)
    {
        return is >> v.ijk_[0] >> v.ijk_[1] >> v.ijk_[2];
    }
    
    friend std::ostream &operator<<(std::ostream &os, vec &v)
    {
        return os << "<" << v.ijk_[0] << "," << v.ijk_[1] << "," << v.ijk_[2] << ">";
    }
    
private:
    double ijk_[3];
};

class point
{
public:
    point() {} // default constructor does nothing, not even inits.
    point(const point &pt)
    {
        xyz_[0] = pt[0];
        xyz_[1] = pt[1];
        xyz_[2] = pt[2];
    }
    point(double x, double y, double z)
    {
        xyz_[0] = x;
        xyz_[1] = y;
        xyz_[2] = z;
    }

    void set(double x, double y, double z)
    {
        xyz_[0] = x;
        xyz_[1] = y;
        xyz_[2] = z;
    }

    double operator[](const int i) const;
    
    // subtracting one point from another returns a vector
    vec operator-(const point &p) const
    {
        return vec(xyz_[0] - p.xyz_[0],
                   xyz_[1] - p.xyz_[1],
                   xyz_[2] - p.xyz_[2]);
    }
    
    point operator+(const vec &v) const
    {
        return point(xyz_[0] + v[0],
                     xyz_[1] + v[1],
                     xyz_[2] + v[2]);
    }
    
    // these two functions are used to allow us to read in
    // and print out (or save to a file). they are used by
    // the parser code.
    friend std::istream &operator>>(std::istream &is, point &p)
    {
        return is >> p.xyz_[0] >> p.xyz_[1] >> p.xyz_[2];
    }
    
    friend std::ostream &operator<<(std::ostream &os, point &p)
    {
        return os << "<" << p.xyz_[0] << "," << p.xyz_[1] << "," << p.xyz_[2] << ">";
    }
private:
    double xyz_[3];
};

class ray
{
public:
    ray() {}
    ray(const point &pt, const vec &dir): _pt(pt), _dir(dir)
    {
        _dir.normalize();
    }
    
    point getPoint() const
    {
        return _pt;
    }
    
    vec getDir() const
    {
        return _dir;
    }
private:
    point _pt;
    vec _dir;
};

inline double vec::operator[](const int i) const
{
    assert(i >= 0 && i < 3);
    return ijk_[i];
}

inline void vec::normalize()
{
    //assert (!(ijk_[0] == 0.0 && ijk_[1] == 0.0 && ijk_[2] == 0.0));
    double len = ijk_[0] * ijk_[0] + 
                 ijk_[1] * ijk_[1] +
                 ijk_[2] * ijk_[2];
    len = sqrt (len);
    if (!len) return;
    ijk_[0] /= len;
    ijk_[1] /= len;
    ijk_[2] /= len;
}

inline double point::operator[](const int i) const
{
    assert(i >= 0 && i < 3);
    return xyz_[i];
}

#endif
