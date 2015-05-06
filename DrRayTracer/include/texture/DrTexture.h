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

/*
 *表面光学特性
 */
struct OpticalProperty {
    //反射+折射+漫反射 <= 1
    DrColor diffuse = BLACK;//漫反射系数
    double specular = 0;//高光反射系数
    double reflection = 0;//镜面反射系数
    double spec_exp = 1,//高光反射指数
    transparency = 0,//透明度
    ambient = 0;//环境反射系数
};

/*
 *纹理类
 */
class DrTexture {
public:
    virtual int getClass() = 0;
    virtual void getProperty(const DrVector& vec, OpticalProperty &property) = 0;
    
};

#endif /* defined(__DrRayTracer__DrTexture__) */
