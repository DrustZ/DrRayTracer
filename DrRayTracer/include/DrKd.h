//
//  DrKd.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/6/29.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrKd__
#define __DrRayTracer__DrKd__

#include <stdio.h>
#include <vector>
#include "DrPnt.h"
#include "DrMath.h"
#include "DrRay.h"
#define LESSTRI 10
#define MAXDEP  20

class DrKd {
public:
    int type;//1 for x, 2 for y, 3 for z
    int size;
    double bestpos;
    int * lxsorted, * bxsorted, * lysorted, * bysorted, * lzsorted, * bzsorted;
    bool leaf,empty;
    double lx,ly,lz,bx,by,bz;
    DrKd * lson;
    DrKd * rson;
    void build(int depth);

    /*
    * 对节点中的三角面片点进行排序，分别以面片x，y，z的最小，最大值排序并存储
    */
    void sort();

    /*
    * SAH Search
    * 计算以某个点为分割左右儿子坐标时的消耗值，选择最小者
    */
    float calculatecost( KdTriInfo tri, double& cost );

    /*
    * 计算以某个坐标分隔后左右儿子的表面积
    */
    double calculateArea(double split, int mode/*1 for left, 2 for right*/);

    /*
    * 二分查找，在一个数组中找到不小于某个数的最大元素位置
    * 用于粗略寻找在某一点左右的面片个数
    */
    int find(double target, int * group, int mode);

    /*
     * 返回与某光线相交的距离，不相交则返回 -1
     * mode：1表示左儿子测试，2表示右儿子测试
     */
    double getIntersect(const DrRay &ray, int mode);

    /*
     *判断是否相交
     */
    bool testIntersect(const DrRay &ray, DrVector &normal, double d);

    /*
     *获得与平面的交点的距离
     */
    double getIntersection(const DrRay &ray, DrVector &normal, double d);

    DrKd(int amount);
};

extern std::vector< KdTriInfo > Triangles;

#endif /* defined(__DrRayTracer__DrKd__) */
