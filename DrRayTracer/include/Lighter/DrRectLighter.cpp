//
//  DrRectLighter.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/29.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrRectLighter.h"

DrRectLighter::DrRectLighter(DrVector &pos, DrColor &col, double inten, DrVector &posright, DrVector &posdown) : m_posright(posright), m_posdown(posdown), DrLighter(pos, col, inten){
    points = new DrVector[2];
    points[0] = posright;
    points[1] = posdown;
    normal = (posright - pos).cross((posdown - pos)).getNormalize();
    d = -normal * pos;
}

DrRectLighter::~DrRectLighter(){
    delete  points;
}

/*
 *判断是否与光源相交并且返回距离，如果不相交，返回-1
 */
double DrRectLighter::inside(const DrRay &v){
    double t = normal * v.direction;
    if (getSign(t) == 0)
        return -1;
    double dist = -(d + v.startpoint.dot(normal)) / t;
    if (getSign(dist) > 0){
        DrVector point = v.getPoint(dist);
        //交点到左右两边的距离之差的绝对值是否等于边长
        double distl = fabs((point - position).cross((points[1] - position).getNormalize()).modulus());
        double distr = fabs((point - points[0]).cross((points[1] - position).getNormalize()).modulus());
        if (getSign(fabs(distr - distl) - fabs((points[0] - position).modulus())) >= 0)
            return -1;
        //交点到上下两边的距离之差的绝对值是否等于边长
        double distu = fabs((point - position).cross((points[0] - position).getNormalize()).modulus());
        double distd = fabs((point - points[1]).cross((points[0] - position).getNormalize()).modulus());
        if (getSign(fabs(distu - distd) - fabs((points[1] - position).modulus())) >= 0)
            return -1;
        return dist;
    }
    return -1;
}
