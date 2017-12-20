//
//  exrIO.cc
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 24/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#include "exrIO.h"

void readRgba(const char fileName[], Imf::Array2D<Imf::Rgba> &pixels, int &width, int &height)
{
    Imf::RgbaInputFile file(fileName);
    Imath::Box2i dw = file.dataWindow();
    
    width = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;
    
    pixels.resizeErase(height, width);
    file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
    file.readPixels(dw.min.y, dw.max.y);
}

void writeRgba(const char fileName[], const Imf::Rgba *pixels, int width, int height)
{
    Imf::RgbaOutputFile file(fileName, width, height, Imf::WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, width);
    file.writePixels(height);
}
