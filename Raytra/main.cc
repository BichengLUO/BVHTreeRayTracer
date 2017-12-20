//
//  main.cpp
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 19/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#include <iostream>
#include "parser.h"
#include "renderer.h"
#include "config.h"
#include "bvhtree.h"

const char *usage = "./raytra [input.scn] [output.exr] [primary_ray_sample] [shadow_ray_sample]";

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << usage << std::endl;
        return -1;
    }
    char *input = argv[1];
    char *output = argv[2];
    int pr_smpl_cnt = atoi(argv[3]);
    int sr_smpl_cnt = atoi(argv[4]);
    Config::getInstance().enableBBoxes(true);
    std::vector<Shape*> objs;
    std::vector<Light*> lights;
    std::vector<Material> materials;
    Light ambient;
    Camera cam;
    Parser parser;
    Renderer renderer;
    
    parser.parse(input, objs, cam, lights, materials, ambient);
    int pw, ph;
    cam.getPixelSize(pw, ph);
    Imf::Array2D<Imf::Rgba> pixels(ph, pw);
    
    BVHTree *bvh = nullptr;
    if (Config::getInstance().bboxesEnabled())
        bvh = new BVHTree(objs);
    renderer.render(objs, lights, materials, ambient, cam, pixels, pr_smpl_cnt, sr_smpl_cnt, bvh);
    
    writeRgba(output, &pixels[0][0], pw, ph);
    return 0;
}
