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
#include "DrPlane.h"
#include "DrSphere.h"
#include "DrLighter.h"
#include "DrRectLighter.h"

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
    
    /*
     *获得相机位置，建立相机坐标系
     */
    void getEyePosition(DrVector &e, DrVector &lookat, DrVector &up);
    
    /*
     * 针对相机坐标系，产生视线，并进行像素点到世界坐标的向量转换
     * x 为 像素第 x 列， y 为 第y行， height 为 投射屏幕与小孔的距离， nx 表示 总列数， ny 表示 总行数
     * left ， right， up， down 分别为 左右上下的坐标值 dist 为眼睛到此像素的距离
     */
    DrRay transformToGlobal(double x, double y, double height, int nx, int ny, double left, double right, double up, double down, double& dist);
};


#endif /* defined(__DrRayTracer__DrScene__) */
