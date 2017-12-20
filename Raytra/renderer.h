//
//  renderer.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef renderer_h
#define renderer_h

#include <vector>
#include "shape.h"
#include "light.h"
#include "camera.h"
#include "exrIO.h"
#include "bvhtree.h"
#include <random>

class Renderer
{
public:
    Renderer() {}
    void render(const std::vector<Shape*> &objs,
                const std::vector<Light*> &lights,
                const std::vector<Material> &materials,
                const Light &ambient,
                const Camera &cam,
                Imf::Array2D<Imf::Rgba> &pixels,
                int pr_smpl_cnt,
                int sr_smpl_cnt,
                const BVHTree *bvh = nullptr);
    
    Imf::Rgba shading(const point &its,
                      const vec &n,
                      const point &eye,
                      const Material &mat,
                      const point &l_pos,
                      const float *l_color);
    
    Imf::Rgba ray_recur(const ray& r,
                        const point &eye,
                        const std::vector<Shape*> &objs,
                        const std::vector<Light*> &lights,
                        const std::vector<Material> &materials,
                        const Light &ambient,
                        int recur_limit);
    Imf::Rgba ray_recur(const ray& r,
                        const point &eye,
                        const std::vector<Shape*> &objs,
                        const std::vector<Light*> &lights,
                        const std::vector<Material> &materials,
                        const Light &ambient,
                        int recur_limit,
                        int sr_smpl_cnt,
                        const BVHTree *bvh,
                        std::random_device &rd);
};

#endif /* renderer_h */
