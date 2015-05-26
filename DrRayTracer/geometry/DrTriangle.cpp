//
//  DrTriangle.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/19.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrTriangle.h"
DrTriangle::DrTriangle(const DrVector &v1, const DrVector &v2, const DrVector &v3, DrPnt<DrTexture>& tex, bool ref, double refr = 0.5, double trans = 1.5): m_v0(v1), m_v1(v2), m_v2(v3), DrGeometry(tex, ref, refr, trans){
    e1 = v2 - v1;
    e2 = v3 - v1;
    normal = e1.cross(e2).getNormalize();
    insert_value = false;
    d = normal * v1;
}

DrTriangle::DrTriangle(const DrVector &v1, const DrVector &v2, const DrVector &v3){
    m_v0 = v1;
    m_v1 = v2;
    m_v2 = v3;
    e1 = v2 - v1;
    e2 = v3 - v1;
    normal = e1.cross(e2).getNormalize();
    insert_value = false;
    d = normal * v1;
}

void DrTriangle::getTex(DrPnt<DrTexture>& tex, bool ref, double refr = 0.5, double trans = 1.5){
    DrGeometry::getTex(tex, refr, refr, trans);
}

double DrTriangle::intersection(const DrRay& ray){
    double t = normal * ray.direction;
    double dist = (d - ray.startpoint.dot(normal)) / t;
    return dist;
}

bool DrTriangle::intersect(const DrRay &ray){
        DrVector P, Q, T;
        double det, u, v;
    
        //Begin calculating determinant - also used to calculate u parameter
        P = ray.direction.cross(e2);
        //if determinant is near zero, ray lies in plane of triangle
        det = e1 * P;
        //NOT CULLING
        if(getSign(det) == 0) return 0;
    
        //calculate distance from V1 to ray origin
        T = ray.startpoint - m_v0;
    
        //Calculate u parameter and test bound
        u = T * P / det;
        //The intersection lies outside of the triangle
        if(getSign(u) < 0 || getSign(u - 1) > 0) return 0;
        
        //Prepare to test v parameter
        Q = T.cross(e1);
        
        //Calculate V parameter and test bound
        v = Q * ray.direction / det;
        //The intersection lies outside of the triangle
        if(getSign(v) < 0 || getSign(u + v - 1) > 0) return 0;
        
        t = e2 * Q / det;
        
        if(getSign(t) > 0) { //ray intersection
            return 1;
        }
        
        // No hit, no win
        return 0;
}

void DrTriangle::getVn(DrVector &vn1, DrVector &vn2, DrVector &vn3){
    normal_v1 = vn1;
    normal_v2 = vn2;
    normal_v3 = vn3;
    insert_value = true;
}


DrVector DrTriangle::getNormal(const DrVector &v){
    if (!insert_value)
        return normal;
    else return normal;
}

bool DrTriangle::getRefraction(DrVector& refraction, const DrVector& point,
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