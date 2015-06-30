//
//  DrGeometry.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrGeometry__
#define __DrRayTracer__DrGeometry__

#include "../DrPnt.h"
#include "../DrRay.h"
#include "../DrColor.h"
#include "../DrMath.h"
#include "../texture/DrTexture.h"
#include <vector>
#include <iostream>

/*
 *基本几何造型类
 */
class DrGeometry {
public:
    //refrand 为漫反射参数
    DrGeometry(DrPnt<DrTexture> tex, bool ref, double refrand, double trans);

    DrGeometry();
    
    virtual ~DrGeometry(){};
    
    void getTex(DrPnt<DrTexture> tex, bool ref, double refrand, double trans);
    
    /*
     *获得在某点法向量
     */
    virtual DrVector getNormal(const DrVector &v) = 0;
    
    /*
     *判断是否在几何体内部
     */
    virtual bool inside (const DrVector& vec) = 0;

    /*
     *判断光线是否在几何体内部
     */
    virtual bool rayInside(const DrRay& ray) = 0;

    /*
     *获得光源到交点的距离
     */
    virtual double intersection(const DrRay& ray) = 0;
    
    /*
     *判断是否相交
     */
    virtual bool intersect(const DrRay& ray) = 0;
    
    /*
     *获得对应光线的折射光线，如果折射光线为全反射或者无折射则返回false
     *refraction：折射光线  point 入射点 view 视线的方向 inside 入射光线是否在几何体内
     */
    virtual bool getRefraction(DrVector& refraction, const DrVector& point, 
                               const DrVector& view, bool inside) = 0;

    virtual int sayMyself() { std::cout << "class : geometry" << std::endl; return 1;}

    /*
     *获得材质
     */
    virtual void getAppearance(DrVector& vec, OpticalProperty& property)
    { m_texture->getProperty(vec, property); }

    virtual DrVector get_v0() const
    { return DrVector(0,0,0); }
    virtual DrVector get_v1() const
    { return DrVector(0,0,0); }
    virtual DrVector get_v2() const
    { return DrVector(0,0,0); }
    
    void setAbsorb(DrColor c)
    { absorb = c; }
    
    DrColor getAbsorb()
    { return absorb; }
    
    /*
     *是否反射
     */
    bool getRef() const
    { return reflective; }
    
    /*
     *获得物体内部的折射率
     */
    bool getTrans() const
    { return transparency > 0; }
    
    double refrand;

protected:
    DrPnt<DrTexture> m_texture;
    bool reflective;
    double transparency;
    DrColor absorb;
    
};

#endif /* defined(__DrRayTracer__DrGeometry__) */
