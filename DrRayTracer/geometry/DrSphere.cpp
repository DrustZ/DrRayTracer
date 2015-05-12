//
//  DrSphere.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrSphere.h"

DrSphere::DrSphere(DrVector& cen, double r, DrPnt<DrTexture>& tex, bool ref, double refr = 0.5, double trans = 1.5) : center(cen), radius(r), DrGeometry(tex, ref, refr, trans){}

bool DrSphere::inside(const DrVector& vec){
    DrVector dir = DrVector(vec, center);
    double dis = dir.modulus();
    if (getSign(fabs(dis - radius)) < 0)
        return true;
    return false;
}

bool DrSphere::rayInside(const DrRay& ray){
    if (inside(ray.startpoint))
        return true;
    if (onface(ray.startpoint))
        if (getNormal(ray.startpoint).dot(ray.direction) < 0)
            return true;
    return false;
}

bool DrSphere::onface(const DrVector& vec){
    DrVector dir = DrVector(vec, center);
    double dis = dir.modulus();
    if (getSign(fabs(dis - radius)) == 0)
        return true;
    return false;
}
        
bool DrSphere::intersect(const DrRay& ray){
    if (inside(ray.startpoint)) return true;
    if (rayInside(ray)) return true;
    DrVector dir = DrVector(ray.startpoint, center);
    float tp = dir * ray.direction;
    
    if (getSign(tp) < 0) return false;
    else {
        float dis = dir.sqrModulus() - sqr(tp);
        if (getSign(dis - sqr(radius)) >= 0)
            return false;
    }
    return true;
}

double DrSphere::intersection(const DrRay& ray){
    DrVector dir = DrVector(ray.startpoint, center);
    double tp = dir * ray.direction;
    double dis = dir.sqrModulus() - sqr(tp);
    double ts = sqrt(sqr(radius) - dis);
    if (inside(ray.startpoint) || onface(ray.startpoint))
        return tp + ts;
    else return tp - ts;
}

void DrSphere::getAppearance(DrVector& vec, OpticalProperty& property){
    double u,v;
    DrVector nvec = DrVector(center, vec);
    if (!getSign(nvec.x) && !getSign(nvec.y)){
        m_texture->getProperty(nvec, property);
    }
    else {
        u = div(atan2(nvec.y, nvec.x), 2 * 3.14159265358979);
        v = div(asin(nvec.z / radius), 2 * 3.14159265358979);
        m_texture->getProperty(DrVector(u,v,0), property);
    }
}

DrVector DrSphere::getNormal(const DrVector& v){
    return (v - center).getNormalize();
}

bool DrSphere::getRefraction(DrVector& refraction, const DrVector& point, 
                              const DrVector& view, bool inside){
    double factor = transparency;
    if (inside) factor = 1 / factor;
//    std::cout <<factor <<std::endl;

    DrVector normal = getNormal(point);
    if (inside) normal = -normal;
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
