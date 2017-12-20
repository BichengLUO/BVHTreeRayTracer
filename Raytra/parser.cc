
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "parser.h"

// assumes you have defined these h files - use other names
// if you have made other files that fit here!
#include "basicmath.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "area_light.h"

// In this code, you can see how Sphere and Camera are set up - you
// will need to do similar for the other classes.
//
// You will also need to keep track of the last material defined, so
// that you can connect it to any surfaces that are subsequently defined.
//
void Parser::parse(const char *file, std::vector<Shape*> &shapes, Camera &cam, std::vector<Light*> &lights, std::vector<Material> &materials, Light &ambient)
{
    std::ifstream in(file);
    char buffer[1025];
    std::string cmd;
    
    int num_cams = 0; // keep track of how many cameras we read in
    int num_light = 0;
    for (int line = 1; in.good(); line++)
    {
        in.getline(buffer, 1024);
        buffer[in.gcount()] = 0;
        cmd = "";
        std::istringstream iss(buffer);
        iss >> cmd;
        if (cmd[0] == '/' || cmd.empty())
        {
            // ignore comments or blank lines
            continue;
        } 
        else if (cmd == "s")
        {
            // Sphere:
            // read in the parameters:
            point pos;
            double r;
            iss >> pos >> r;
            // construct it:
            // at the end of this loop, we put any newly-constructed surfaces
            // into the surfaceVector.
            shapes.push_back(new Sphere(pos, r, materials.size() - 1));
        } 
        else if (cmd == "t")
        {
            point a, b, c;
            iss >> a >> b >> c;
            shapes.push_back(new Triangle(a, b, c, materials.size() - 1));
        }
        else if (cmd == "w")
        {
            std::string path;
            iss >> path;
            std::vector<int> tris;
            std::vector<Triangle*> triangles;
            std::vector<float> verts;
            read_wavefront_file(path, tris, verts);
            std::vector<vec> vert_normals(verts.size() / 3, vec(0, 0, 0));
            for (int i = 0; i < tris.size(); i += 3)
            {
                point a, b, c;
                int p1_id = tris[i], p2_id = tris[i + 1], p3_id = tris[i + 2];
                a = point(verts[3 * p1_id], verts[3 * p1_id + 1], verts[3 * p1_id + 2]);
                b = point(verts[3 * p2_id], verts[3 * p2_id + 1], verts[3 * p2_id + 2]);
                c = point(verts[3 * p3_id], verts[3 * p3_id + 1], verts[3 * p3_id + 2]);
                Triangle *tri = new Triangle(a, b, c, materials.size() - 1);
                triangles.push_back(tri);
                vert_normals[p1_id] += tri->getNormal();
                vert_normals[p2_id] += tri->getNormal();
                vert_normals[p3_id] += tri->getNormal();
            }
            for (int i = 0; i < tris.size(); i += 3)
            {
                int p1_id = tris[i], p2_id = tris[i + 1], p3_id = tris[i + 2];
                triangles[i / 3]->vert_normal[0] = vert_normals[p1_id];
                triangles[i / 3]->vert_normal[1] = vert_normals[p2_id];
                triangles[i / 3]->vert_normal[2] = vert_normals[p3_id];
                triangles[i / 3]->vert_normal[0].normalize();
                triangles[i / 3]->vert_normal[1].normalize();
                triangles[i / 3]->vert_normal[2].normalize();
            }
            shapes.insert(shapes.end(), triangles.begin(), triangles.end());
        }
        else if (cmd == "p")
        {
            // Plane
            vec normal;
            double scalar;
            iss >> normal >> scalar;
            shapes.push_back(new Plane(normal, scalar, materials.size() - 1));
        }
        else if (cmd == "c")
        {
            // Camera:
            ++num_cams; // keep track of how many we read in
            // read in the parameters:
            point pos;
            vec dir;
            double d, iw, ih;
            int pw, ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
            // construct it:
            cam.init(pos, dir, d, iw, ih, pw, ph);
        } 
        else if (cmd == "l")
        {
            ++num_light;
            char tp;
            iss >> tp;
            Light l;
            if (tp == 's')
            {
                point pos;
                vec dir;
                vec u_dir;
                double len;
                float r, g, b;
                iss >> pos >> dir >> u_dir >> len >> r >> g >> b;
                lights.push_back(new AreaLight(pos, dir, u_dir, len, r, g, b));
            }
            else if (tp == 'p')
            {
                point pos;
                float r, g, b;
                iss >> pos >> r >> g >> b;
                lights.push_back(new Light(tp, pos, r, g, b));
            }
        }
        else if (cmd == "m")
        {
            float dr, dg, db, sr, sg, sb, ir, ig, ib;
            double r;
            iss >> dr >> dg >> db >> sr >> sg >> sb >> r >> ir >> ig >> ib;
            materials.push_back(Material(dr, dg, db, sr, sg, sb, r, ir, ig, ib));
        }
        else
        {
            std::cerr << "Parser error: invalid command at line " << line << std::endl;
        }
    }
    in.close();
    // make sure we read in 1 camera, no more no less).
    if (num_cams != 1)
        std::cerr << "scene file error: exactly ONE camera must be defined." << std::endl;
    if (!num_light)
        std::cerr << "scene file error: at least ONE light must be defined." << std::endl;
}

void Parser::read_wavefront_file(const std::string &file,
                                 std::vector<int> &tris,
                                 std::vector<float> &verts)
{
    // clear out the tris and verts vectors:
    tris.clear();
    verts.clear();
    
    std::ifstream in(file);
    char buffer[1025];
    std::string cmd;
    
    for (int line = 1; in.good(); line++)
    {
        in.getline(buffer, 1024);
        buffer[in.gcount()] = 0;
        cmd = "";
        std::istringstream iss (buffer);
        iss >> cmd;
        if (cmd[0] == '#' || cmd.empty())
        {
            // ignore comments or blank lines
            continue;
        }
        else if (cmd == "v")
        {
            // got a vertex:
            // read in the parameters:
            double pa, pb, pc;
            iss >> pa >> pb >> pc;
            
            verts.push_back(pa);
            verts.push_back(pb);
            verts.push_back(pc);
        }
        else if (cmd == "f")
        {
            std::string p1, p2, p3;
            iss >> p1 >> p2 >> p3;
            std::istringstream issp1(p1);
            std::istringstream issp2(p2);
            std::istringstream issp3(p3);
            int i1, i2, i3;
            issp1 >> i1;
            issp2 >> i2;
            issp3 >> i3;
            tris.push_back(i1 - 1);
            tris.push_back(i2 - 1);
            tris.push_back(i3 - 1);
        }
    }
    in.close();
}
