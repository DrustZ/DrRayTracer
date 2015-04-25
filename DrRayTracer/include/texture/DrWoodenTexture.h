//
//  DrWoodenTexture.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrWoodenTexture__
#define __DrRayTracer__DrWoodenTexture__

#include "DrTexture.h"

class DrWoodenTexture : public DrTexture {
    OpticalProperty wood_deep, wood_shallow;
    double m_length, m_width;

    public:
    DrWoodenTexture(const OpticalProperty &deep,
                  const OpticalProperty &shallow,
                  double length = 60, double width = 40):
    wood_deep(deep), wood_shallow(shallow),
    m_length(length), m_width(width){}

    void getProperty(const DrVector& vec, OpticalProperty &property);
};
#endif /* defined(__DrRayTracer__DrWoodenTexture__) */
