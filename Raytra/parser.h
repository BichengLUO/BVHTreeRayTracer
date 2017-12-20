#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>

#include "camera.h"
#include "shape.h"
#include "light.h"

//
// Basic parser for the scene files:
//
class Parser
{
public:
    void parse(const char *file, std::vector<Shape*> &shapes, Camera &cam, std::vector<Light*> &lights, std::vector<Material> &materials, Light &ambient);
    
    void read_wavefront_file(const std::string &file,
                             std::vector<int> &tris,
                             std::vector<float> &verts);
};

#endif
