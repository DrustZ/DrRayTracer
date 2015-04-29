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

/*
 *处处一致的纹理
 */
class DrUniformTexture : public DrTexture {
    OpticalProperty m_property;
    
public:
    DrUniformTexture(OpticalProperty &prpt) : m_property(prpt) {}

    int getClass()
    { return 1; }
    
    void getProperty(const DrVector& vec, OpticalProperty &property)
    {
        property = m_property;
    }
};

#endif /* defined(__DrRayTracer__DrUniformTexture__) */
