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
#include "DrSphere.h"
#include "DrLighter.h"

class DrScene
{
    std::vector<DrPnt<DrGeometry> > objs;
    std::vector<DrLighter> lights;
    int max_dep, min_weight;
    DrColor m_ambient;
    DrVector eye, u, v, w;
    
public:
    
    DrScene(int dep, int weight, DrColor &ambinet);
    ~DrScene();
    
    DrColor doRayTracing(DrRay &ray, double weight, int depth);
    
    void addObj(DrPnt<DrGeometry> &obj){
        objs.push_back(obj);
    }
    
    void addlights(DrLighter &light){
        lights.push_back(light);
    }
    
    bool testShadow(DrRay &ray, double max_dist);
    
    double getInsection(DrRay &ray, DrPnt<DrGeometry> &pnt);
    
    void getEyePosition(DrVector &e, DrVector &lookat, DrVector &up);
    
    DrRay transformToGlobal(int i, int j, double height);
};


#endif /* defined(__DrRayTracer__DrScene__) */
