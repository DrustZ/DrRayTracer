//
//  DrTexture.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrTexture__
#define __DrRayTracer__DrTexture__

#include "DrMath.h"
#include "DrPnt.h"

struct OpticalProperty {
    DrColor diffuse = BLACK;//漫反射系数
    int specular = 0,//镜面反射系数
    spec_exp = 1,//镜面反射指数
    transparency = 0,//透明度
    ambient = 0;//环境反射系数
};


class DrTexture {
public:
    virtual void getProperty(DrVector& vec, OpticalProperty &property) = 0;
    
};

#endif /* defined(__DrRayTracer__DrTexture__) */
