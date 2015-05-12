//
//  DrPlane.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrPlane__
#define __DrRayTracer__DrPlane__

#include "DrGeometry.h"

class DrPlane : public DrGeometry {
public:
    DrPlane(const DrVector &norm, double t, DrPnt<DrTexture> tex, bool ref, double refr, double trans);
    
    /*
     *获得在某点法向量
     */
    DrVector getNormal(const DrVector &v);
    
    /*
     *判断是否在几何体内部
     */
    bool inside (const DrVector& vec)
    { return getSign(vec.dot(normal) - d) == 0; }
    
    /*
     *判断光线是否在几何体内部
     */
    bool rayInside(const DrRay& ray)
    { return false; }
    
    /*
     *获得光源到交点的距离
     */
    double intersection(const DrRay& ray);
    
    /*
     *判断是否相交
     */
    bool intersect(const DrRay& ray);
    
    /*
     *获得对应光线的折射光线，如果折射光线为全反射或者无折射则返回false
     *refraction：折射光线  point 入射点 view 视线的方向 inside 入射光线是否在几何体内
     */
    bool getRefraction(DrVector& refraction, const DrVector& point,
                               const DrVector& view, bool inside);
    
    void getAppearance(DrVector& vec, OpticalProperty& property);

    void sayMyself() { std::cout << "class : plane" << std::endl; }

private:
    DrVector normal;
    double d;
};

#endif /* defined(__DrRayTracer__DrPlane__) */
