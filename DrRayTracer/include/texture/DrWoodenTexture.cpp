//
//  DrWoodenTexture.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrWoodenTexture.h"
void DrWoodenTexture::getProperty(const DrVector& vec, OpticalProperty &property){
    double r = sqrt(sqr(vec.x) + sqr(vec.y));
    double a = vec.x + vec.y;
    r = r + 2 * sin(20 * a);
    r = r + 2 * sin(20 * a + vec.z / 150);
    
    double grain = fmod(r, m_length);
    property = (grain > m_width) ? wood_deep : wood_shallow;
}
