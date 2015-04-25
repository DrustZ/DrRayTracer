//
//  DrLighter.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrLighter__
#define __DrRayTracer__DrLighter__

#include "DrMath.h"

/*
 *光源类
 */
class DrLighter {
public:
    DrVector position;//光源位置
    DrColor color;//光源颜色
    double intensity;//光源强度
    
    DrLighter(DrVector &pos, DrColor &col, double inten) : position(pos), color(col), intensity(inten) {}
    
    DrVector generateLighter(double r = 3);
};

#endif /* defined(__DrRayTracer__DrLighter__) */
