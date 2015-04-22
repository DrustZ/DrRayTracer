//
//  DrRay.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrRay__
#define __DrRayTracer__DrRay__

#include "DrMath.h"
#include "DrRay.h"

class DrRay {
public:
    DrVector startpoint, direction;
    DrRay(DrVector& start, DrVector& dir);
    
    DrVector getPoint(double dist) const
    { return DrVector(startpoint.x + direction.x * dist, startpoint.y + direction.y * dist, startpoint.z + direction.z * dist); }
    
};

#endif /* defined(__DrRayTracer__DrRay__) */
