//
//  renderer.cc
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 29/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#include "renderer.h"
#include <limits>
#include <algorithm>
#include "progressbar.h"
#include "config.h"
#include "area_light.h"
#include <tbb/tbb.h>

void Renderer::render(const std::vector<Shape*> &objs,
                      const std::vector<Light*> &lights,
                      const std::vector<Material> &materials,
                      const Light &ambient,
                      const Camera &cam,
                      Imf::Array2D<Imf::Rgba> &pixels,
                      int pr_smpl_cnt,
                      int sr_smpl_cnt,
                      const BVHTree *bvh)
{
    double iw, ih;
    cam.getImageSize(iw, ih);
    double focal_len = cam.getFocalLen();
    point eye = cam.getEyePos();
    vec dir = cam.getDir();
    vec up = cam.getUp();
    vec right = cam.getRight();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0);
    int psc = pr_smpl_cnt;
    if (!psc) psc = 1;
    int counter;
    tbb::mutex mutex;
    tbb::parallel_for(tbb::blocked_range<int>(0, pixels.height()), [&](const tbb::blocked_range<int> &r)
    {
        for (int y = r.begin(); y != r.end(); y++)
        for (int x = 0; x < pixels.width(); x++)
        {
            {
                tbb::mutex::scoped_lock lock(mutex);
                counter++;
                progress(counter, pixels.height() * pixels.width());
            }
            pixels[y][x] = Imf::Rgba(0, 0, 0);
            for (int p = 0; p < psc; p++)
            for (int q = 0; q < psc; q++)
            {
                Imf::Rgba pix(0, 0, 0);
                double y_p = 0.5;
                double x_q = 0.5;
                if (pr_smpl_cnt)
                {
                    y_p = (p + dis(gen)) / psc;
                    x_q = (q + dis(gen)) / psc;
                }
                double v = -ih / 2.0 + ih * (pixels.height() - (y + y_p)) / pixels.height();
                double u = -iw / 2.0 + iw * (x + x_q) / pixels.width();
                vec ray_dir = (right * u) + (up * v) + (dir * focal_len);
                ray r(eye, ray_dir);
                if (bvh)
                    pix = ray_recur(r, eye, objs, lights, materials, ambient, 5, sr_smpl_cnt, bvh, rd);
                else
                    pix = ray_recur(r, eye, objs, lights, materials, ambient, 5);
                pixels[y][x].r += pix.r;
                pixels[y][x].g += pix.g;
                pixels[y][x].b += pix.b;
            }
            pixels[y][x].r /= psc * psc;
            pixels[y][x].g /= psc * psc;
            pixels[y][x].b /= psc * psc;
        }
    });
}

Imf::Rgba Renderer::ray_recur(const ray& r,
                              const point &eye,
                              const std::vector<Shape*> &objs,
                              const std::vector<Light*> &lights,
                              const std::vector<Material> &materials,
                              const Light &ambient,
                              int recur_limit)
{
    Imf::Rgba pix(0, 0, 0);
    if (!recur_limit) return pix;
    point its; vec normal, shd_normal;
    double t = std::numeric_limits<double>::max();
    int its_ind = -1;
    for (int i = 0; i < objs.size(); i++)
    {
        point its_t; vec normal_t, shd_normal_t; double t_t;
        if (objs[i]->intersect(r, its_t, normal_t, shd_normal_t, t_t) && t_t < t)
        {
            its = its_t;
            normal = normal_t;
            shd_normal = shd_normal_t;
            t = t_t;
            its_ind = i;
        }
    }
    if (its_ind == -1) return pix;
    Material mat = materials[objs[its_ind]->getMaterialID()];
    for (int i = 0; i < lights.size(); i++)
    {
        ray shdr(its, lights[i]->getPos() - its);
        vec v = eye - its;
        v.normalize();
        if (v.dotProduct(normal) * shdr.getDir().dotProduct(normal) < 0) continue;
        double max_t = (lights[i]->getPos() - its).length();
        bool shadow = false;
        for (int j = 0; j < objs.size(); j++)
        {
            double t;
            if (objs[j]->isIntersectedBy(shdr, t) && t > EPS && t < max_t)
            {
                shadow = true;
                break;
            }
        }
        if (!shadow)
        {
            Imf::Rgba sh = shading(its, shd_normal, eye, mat, lights[i]->getPos(), lights[i]->color);
            pix.r += sh.r;
            pix.g += sh.g;
            pix.b += sh.b;
        }
    }
    if (mat.ideal_spec[0] != 0 || mat.ideal_spec[1] != 0 || mat.ideal_spec[2] != 0)
    {
        double dn2 = 2 * r.getDir().dotProduct(normal);
        vec reflect = r.getDir() - normal * dn2;
        reflect.normalize();
        its = its + reflect * EPS;
        ray reflect_ray(its, reflect);
        Imf::Rgba refl_pix = ray_recur(reflect_ray, its, objs, lights, materials, ambient, recur_limit - 1);
        pix.r += mat.ideal_spec[0] * refl_pix.r;
        pix.g += mat.ideal_spec[1] * refl_pix.g;
        pix.b += mat.ideal_spec[2] * refl_pix.b;
    }
    return pix;
}

Imf::Rgba Renderer::ray_recur(const ray& r,
                              const point &eye,
                              const std::vector<Shape*> &objs,
                              const std::vector<Light*> &lights,
                              const std::vector<Material> &materials,
                              const Light &ambient,
                              int recur_limit,
                              int sr_smpl_cnt,
                              const BVHTree *bvh,
                              std::random_device &rd)
{
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.0);
    Imf::Rgba pix(0, 0, 0);
    if (!recur_limit) return pix;
    point its; vec normal, shd_normal;
    double t;
    Shape *obj = bvh->hit(r);
    if (!obj) return pix;
    if (!obj->intersect(r, its, normal, shd_normal, t)) return pix;
    Material mat = materials[obj->getMaterialID()];
    for (int i = 0; i < lights.size(); i++)
    {
        if (lights[i]->getType() == 'p')
        {
            ray shdr(its, lights[i]->getPos() - its);
            vec v = eye - its;
            v.normalize();
            if (v.dotProduct(normal) * shdr.getDir().dotProduct(normal) < 0) continue;
            double max_t = (lights[i]->getPos() - its).length();
            Shape *block = bvh->hit(shdr);
            if (block && block->isIntersectedBy(shdr, t) && t > EPS && t < max_t) continue;
            Imf::Rgba sh = shading(its, shd_normal, eye, mat, lights[i]->getPos(), lights[i]->color);
            pix.r += sh.r;
            pix.g += sh.g;
            pix.b += sh.b;
        }
        else if (lights[i]->getType() == 's')
        {
            AreaLight *al = (AreaLight*)lights[i];
            Imf::Rgba sh_pix(0, 0, 0);
            int ssc = sr_smpl_cnt;
            if (!ssc) ssc = 1;
            for (int p = 0; p < ssc; p++)
            for (int q = 0; q < ssc; q++)
            {
                double up = -0.5, vq = -0.5;
                if (sr_smpl_cnt)
                {
                    up += (p + dis(gen)) / ssc;
                    vq += (q + dis(gen)) / ssc;
                }
                point l_pos = al->getPos() + al->getUDir() * up * al->getLen() + al->getVDir() * vq * al->getLen();
                ray shdr(its, l_pos - its);
                vec v = eye - its;
                v.normalize();
                if (v.dotProduct(normal) * shdr.getDir().dotProduct(normal) < 0) continue;
                double max_t = (l_pos - its).length();
                Shape *block = bvh->hit(shdr);
                if (block && block->isIntersectedBy(shdr, t) && t > EPS && t < max_t) continue;
                Imf::Rgba sh = shading(its, shd_normal, eye, mat, l_pos, al->color);
                vec il = shdr.getDir().invert();
                double coef = std::max(0., al->getDir().dotProduct(il));
                sh_pix.r += coef * sh.r;
                sh_pix.g += coef * sh.g;
                sh_pix.b += coef * sh.b;
            }
            pix.r += sh_pix.r / (ssc * ssc);
            pix.g += sh_pix.g / (ssc * ssc);
            pix.b += sh_pix.b / (ssc * ssc);
        }
    }
    if (mat.ideal_spec[0] != 0 || mat.ideal_spec[1] != 0 || mat.ideal_spec[2] != 0)
    {
        double dn2 = 2 * r.getDir().dotProduct(normal);
        vec reflect = r.getDir() - normal * dn2;
        reflect.normalize();
        its = its + reflect * EPS;
        ray reflect_ray(its, reflect);
        Imf::Rgba refl_pix = ray_recur(reflect_ray, its, objs, lights, materials, ambient, recur_limit - 1, sr_smpl_cnt, bvh, rd);
        pix.r += mat.ideal_spec[0] * refl_pix.r;
        pix.g += mat.ideal_spec[1] * refl_pix.g;
        pix.b += mat.ideal_spec[2] * refl_pix.b;
    }
    return pix;
}

Imf::Rgba Renderer::shading(const point &its, const vec &n, const point &eye, const Material &mat, const point &l_pos, const float *l_color)
{
    vec v = eye - its;
    v.normalize();
    vec l = l_pos - its;
    double d2 = l.dotProduct(l);
    l.normalize();
    vec h = v + l;
    h.normalize();
    float r, g, b;
    if (n.dotProduct(v) >= 0)
    {
        double dif_coef = std::max(0., n.dotProduct(l));
        double spec_coef = pow(std::max(0., n.dotProduct(h)), mat.getPhongExp());
        r = l_color[0] / d2 * (mat.diffuse[0] * dif_coef + mat.specular[0] * spec_coef);
        g = l_color[1] / d2 * (mat.diffuse[1] * dif_coef + mat.specular[1] * spec_coef);
        b = l_color[2] / d2 * (mat.diffuse[2] * dif_coef + mat.specular[2] * spec_coef);
    }
    else
    {
        double dif_coef = n.invert().dotProduct(l);
        r = l_color[0] / d2 * dif_coef;
        g = l_color[1] / d2 * dif_coef;
        b = 0;
    }
    return Imf::Rgba(r, g, b);
}
