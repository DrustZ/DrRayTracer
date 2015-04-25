//
//  DrPlane.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrPlane.h"

DrPlane::DrPlane(const DrVector &norm, double t, DrPnt<DrTexture> tex, bool ref, double trans = 1.5) : normal(norm), d(t), DrGeometry(tex, ref, trans){}

DrVector DrPlane::getNormal(const DrVector &v)
{ return normal; }

/*
 *获得光源到交点的距离
 */
double DrPlane::intersection(const DrRay& ray){
    double t = normal * ray.direction;
    double dist = (d - ray.startpoint.dot(normal)) / t;
    return dist;
}

/*
 *判断是否相交
 */
bool DrPlane::intersect(const DrRay& ray){
    double t = normal * ray.direction;
    if (getSign(t) == 0)
        return false;
    if (getSign(intersection(ray)) < 0) return false;
    return true;
}

/*
 *获得对应光线的折射光线，如果折射光线为全反射或者无折射则返回false
 *refraction：折射光线  point 入射点 view 视线的方向 inside 入射光线是否在几何体内
 */
bool DrPlane::getRefraction(DrVector& refraction, const DrVector& point,
                   const DrVector& view, bool inside){
    return false;
}

