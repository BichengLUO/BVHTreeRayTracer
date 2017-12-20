//
//  progressbar.cc
//  Raytra
//
//  Created by Eagle Sky on 09/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#include "progressbar.h"
#include <iostream>

void progress(int processed_cnt, int total_cnt)
{
    int progress = 20 * processed_cnt++ / total_cnt;
    std::cout << "\r" << processed_cnt << "/" << total_cnt << ":[";
    for (int i = 0; i < progress; i++) std::cout << "=";
    for (int i = 0; i < 20 - progress; i++) std::cout << " ";
    printf("]%.3f%%\t", 100 * processed_cnt / float(total_cnt));
}
