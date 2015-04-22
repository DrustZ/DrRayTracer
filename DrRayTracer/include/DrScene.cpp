//
//  DrScene.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrScene.h"
//
// Created by 明瑞 on 15/4/21.
// Copyright (c) 2015 明瑞. All rights reserved.
//
#include "DrScene.h"

DrScene::DrScene(int dep, int weight, DrColor &ambinet) : max_dep(dep), min_weight(weight), m_ambient(ambinet){
    objs = std::vector<DrPnt<DrGeometry> >();
    lights = std::vector<DrLighter>();
}

DrScene::~DrScene(){
}

DrColor DrScene::doRayTracing(DrRay &ray, double weight, int depth){
    if (depth > max_dep) return BLACK;
    if (weight < min_weight) return BLACK;
    
    DrPnt<DrGeometry> pnt = DrPnt<DrGeometry>(NULL);
    double dist = getInsection(ray, pnt);
    if (!pnt) return BLACK;
    
    DrVector point = ray.getPoint(dist);
     OpticalProperty prop;
     pnt->getAppearance(point, prop);
     DrVector norm = pnt->getNormal(point);
    
    DrColor color;
    
    for (const auto &i: lights){
        DrVector l_to_p = point - i.position;
        double maxdis = l_to_p.modulus();
        l_to_p.normalize();
        
        DrVector pos = i.position;
        DrRay lray = DrRay(pos, l_to_p);
        if (testShadow(lray, maxdis))
            continue;
        
        color += DrPhongShader::get_shade(point, prop, norm, ray, i, m_ambient);
    }
    
    color.setToRange();

    return color;
}

bool DrScene::testShadow(DrRay &ray, double max_dist){
    for (const auto &i: objs){
        if (i->intersect(ray)){
            double dis = i->intersection(ray);
            if (dis < max_dep)
                return true;
        }
    }
    return false;
}

double DrScene::getInsection(DrRay &ray, DrPnt<DrGeometry> &pnt){
    double mindis = std::numeric_limits<double>::max();
    for (const auto &i: objs){
        if (i->intersect(ray)){
            //i->sayMyself();
            double dis = i->intersection(ray);
            if (dis < mindis){
                mindis = dis;
                pnt = i;
            }
        }
    }
    return mindis;
}

void DrScene::getEyePosition(DrVector &e, DrVector &lookat, DrVector &up){
    w = lookat.getNormalize();
    u = up.cross(w).getNormalize();
    v = w.cross(u);
    eye = e;
    //std::cout << w << u << v;
}

DrRay DrScene::transformToGlobal(int i, int j, double height){
    DrVector vec = u * i + v * j + w * height;
    //std::cout << vec << std::endl;
    return DrRay(eye, vec);
}





