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

DrScene::DrScene(int dep, int weight,const DrColor &ambinet) : max_dep(dep), min_weight(weight), m_ambient(ambinet){
    objs = std::vector<DrPnt<DrGeometry> >();
    lights = std::vector<DrLighter>();
}

DrScene::~DrScene(){
}

DrColor DrScene::doRayTracing(const DrRay &ray, double weight, int depth){
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
//    std::cout << ray.direction << std::endl;
    for (auto &i: lights){
//        for (int j = 0; j < 5; ++j){
//            DrLighter temp_lighter = i;
////            temp_lighter.position = i.generateLighter();
////            temp_lighter.intensity = i.intensity / 5;
            DrVector l_to_p = point - i.position;
            double maxdis = l_to_p.modulus();
            l_to_p.normalize();
            
            DrVector pos = i.position;
            DrRay lray = DrRay(pos, l_to_p);
            if (testShadow(lray, maxdis)){
                continue;
            }
            
            color += DrPhongShader::get_shade(point, prop, norm, ray, i, m_ambient);
        //}
    }

//   if (prop.reflection > 0 && pnt->getRef()){
//       DrVector ref = -ray.direction.reflection(-norm);
//       color += doRayTracing(DrRay(point, ref),
//                             weight * prop.reflection, 1+depth) * prop.reflection;
//   }
//    if (depth > 2) std::cout << "ca" << std::endl;
    
    if (prop.transparency){
        DrVector trans;
        if (pnt->getRefraction(trans, point, ray.direction, pnt->rayInside(ray))){
//            std::cout << "depth" << depth << std::endl;
            DrRay frecRay = DrRay(point,trans);
            //Beer Law
            double dist = pnt->intersection(frecRay); //计算光走过的距离
            DrColor absor = pnt->getAbsorb() * 0.15 * -dist; //一个参数，与物体对光的吸收和路径长度的相反数呈正比
            DrColor trans = DrColor( exp( absor.r ) , exp( absor.g ) , exp( absor.b ) ); //用指数函数，计算能通过的光的比例
            
            color += doRayTracing(frecRay, weight * prop.transparency, depth + 1) * prop.transparency * trans;
        }
    }
    color.setToRange();

    return color;
}

bool DrScene::testShadow(const DrRay &ray, double max_dist){
    for (const auto &i: objs){
        if (i->intersect(ray)){
            double dis = i->intersection(ray);
            if (getSign(dis-max_dist) < 0)
                return true;
        }
    }
    return false;
}

double DrScene::getInsection(const DrRay &ray, DrPnt<DrGeometry> &pnt){
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
}

DrRay DrScene::transformToGlobal(int x, int y, double height, 
    int nx, int ny, double left, double right, double up, double down){
    double us = left + (right - left) * ((double)x + 0.5) / nx;
    double vs = down + (up - down) * ((double)y + 0.5) / ny;
    DrVector vec = u * us + v * vs + w * height;
//    std::cout << vec << std::endl;
    return DrRay(eye, vec);
}





