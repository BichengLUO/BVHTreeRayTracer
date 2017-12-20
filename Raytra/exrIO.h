//
//  exrIO.h
//  COMS 4160 Computer Graphics, Raytra
//
//  Created by Bicheng Luo on 24/09/2017.
//  Copyright © 2017 Bicheng Luo. All rights reserved.
//

#ifndef exrIO_h
#define exrIO_h

#include <ImfArray.h>
#include <ImfRgbaFile.h>

void readRgba(const char fileName[], Imf::Array2D<Imf::Rgba> &pixels, int &width, int &height);
void writeRgba(const char fileName[], const Imf::Rgba *pixels, int width, int height);

#endif /* exrIO_hpp */
