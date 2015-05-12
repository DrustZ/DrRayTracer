//
//  DrRay.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrRay.h"

DrRay::DrRay(DrVector& start, DrVector& dir){
    startpoint = start;
    direction = dir.getNormalize();
}
