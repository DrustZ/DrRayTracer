//
//  DrLighter.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrLighter.h"
#include <ctime>

#define random(x) (rand()%x)

DrVector DrLighter::generateLighter(double r){
    srand((int)time(0));
    DrVector l_dir = DrVector(random(10),random(10),random(10));
    l_dir = l_dir.getNormalize() * r;
    return position + l_dir;
}