//
//  DrPlane.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrPlane.h"

DrPlane::DrPlane(const DrVector &norm, double t, DrPnt<DrTexture> tex, bool ref, double refr = 0.5, double trans = 1.5) : normal(norm), d(t), DrGeometry(tex, ref, refr, trans){}

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
    if (getSign( normal * ray.startpoint) == 0) return false;
    if (getSign(intersection(ray)) <= 0) return false;
    return true;
}


void DrPlane::getAppearance(DrVector& vec, OpticalProperty& property){
    if (m_texture->getClass() > 0)
        m_texture->getProperty(vec, property);
    else {
        double u,v;
        u = (vec.x + 500)/1000;
        v = (vec.y + 500)/1000;
        m_texture->getProperty(DrVector(u,v,0), property);
    }
}
/*
 *获得对应光线的折射光线，如果折射光线为全反射或者无折射则返回false
 *refraction：折射光线  point 入射点 view 视线的方向 inside 入射光线是否在几何体内
 */
bool DrPlane::getRefraction(DrVector& refraction, const DrVector& point,
                   const DrVector& view, bool inside){
    double factor = transparency;
    if (inside) factor = 1 / factor;
    //    std::cout <<factor <<std::endl;
    
    DrVector normal = getNormal(point);
    if (normal * view > 0) normal = -normal;
    double cos1 = -view * normal;
    //    std::cout << "cos1  " << cos1 << std::endl;
    double cos2 =  sqrt(1-1/sqr(factor)*(1-sqr(cos1)));
    if (cos2 > 0){
        refraction = view;
        refraction = refraction * (1/factor) - normal * (cos2 - 1/factor * cos1);
        return true;
    } else
        return false;
}

