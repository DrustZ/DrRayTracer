//
//  DrUniformTexture.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrUniformTexture__
#define __DrRayTracer__DrUniformTexture__
#include "DrTexture.h"

class DrUniformTexture : public DrTexture {
    OpticalProperty m_property;
    
public:
    DrUniformTexture(OpticalProperty &prpt) : m_property(prpt) {}
    
    void getProperty(DrVector& vec, OpticalProperty &property)
    {
        property = m_property;
    }
};

#endif /* defined(__DrRayTracer__DrUniformTexture__) */
