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

class DrLighter {
public:
    DrVector position;
    DrColor color;
    double intensity;
    
    DrLighter(DrVector &pos, DrColor &col, double inten) : position(pos), color(col), intensity(inten) {}
    
};

#endif /* defined(__DrRayTracer__DrLighter__) */
