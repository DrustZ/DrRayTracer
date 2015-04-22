//
//  DrSphere.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrSphere.h"

DrSphere::DrSphere(DrVector& cen, double r, DrPnt<DrTexture>& tex) : center(cen), radius(r), DrGeometry(tex){}

bool DrSphere::inside(DrVector& vec){
    DrVector dir = DrVector(vec, center);
    double dis = dir.modulus();
    if (getSign(fabs(dis - radius)) < 0)
        return true;
    return false;
}
        
bool DrSphere::onface(DrVector& vec){
    DrVector dir = DrVector(vec, center);
    double dis = dir.modulus();
    if (getSign(fabs(dis - radius)) == 0)
        return true;
    return false;
}
        
bool DrSphere::intersect(DrRay& ray){
    if (inside(ray.startpoint)) return true;
    
    DrVector dir = DrVector(ray.startpoint, center);
    float tp = dir * ray.direction;
    
    if (getSign(tp) < 0) return false;
    else {
        float dis = dir.sqrModulus() - sqr(tp);
        //std::cout << dir << ray.direction << tp << " " << dis << std::endl;
        if (getSign(dis - sqr(radius)) > 0)
            return false;
    }
    return true;
}

double DrSphere::intersection(DrRay& ray){
    DrVector dir = DrVector(ray.startpoint, center);
    double tp = dir * ray.direction;
    double dis = dir.sqrModulus() - sqr(tp);
    double ts = sqrt(sqr(radius) - dis);
    if (!inside(ray.startpoint)) 
        return tp - ts;
    else return tp + ts;
}

DrVector DrSphere::getNormal(DrVector& v){
    return (center - v).getNormalize();
}

