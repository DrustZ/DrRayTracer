//
//  DrScene.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrScene__
#define __DrRayTracer__DrScene__
#include "DrPhongShader.h"
#include "geometry/DrPlane.h"
#include "geometry/DrSphere.h"
#include "Lighter/DrLighter.h"
#include "Lighter/DrRectLighter.h"

/*
 *场景类
 *一个场景即涵盖一张图片，包括各种必须要素
 */
class DrScene
{
    std::vector<DrPnt<DrGeometry> > objs;
    std::vector<DrPnt<DrLighter>> lights;
    int max_dep, min_weight;
    DrColor m_ambient;
    DrVector eye, u, v, w;
    
public:
    
    DrScene(int dep, int weight, const DrColor &ambinet);
    ~DrScene();
    
    /*
     *进行 光线追踪
     */
    DrColor doRayTracing(const DrRay &ray, double weight, int depth, int& route);
    
    /*
     *添加物体
     */
    void addObj(DrPnt<DrGeometry> &obj){
        objs.push_back(obj);
    }
    
    /*
     *添加光源
     */
    void addlights(DrPnt<DrLighter> &light){
        lights.push_back(light);
    }
    
    /*
     *判断遮挡
     *ray为光源到到该点的向量， max_dis 表示光源到该点的距离
     */
    bool testShadow(const DrRay &ray, double max_dist);
    
    /*
     *获得交点 
     *ray 为视线， pnt 为与实现相交的物体
     *返回值为交点到视点的距离
     */
    double getInsection(const DrRay &ray, DrPnt<DrGeometry> &pnt, int& idx);
   
};


#endif /* defined(__DrRayTracer__DrScene__) */
