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
    lights = std::vector<DrPnt<DrLighter> >();
}

DrScene::~DrScene(){
}

DrColor DrScene::doRayTracing(const DrRay &ray, double weight, int depth, int& route){
    using std::cout;
    using std::endl;
    
    if (depth > max_dep) return BLACK;
    if (weight < min_weight) return BLACK;
    int index = 0;
    DrPnt<DrGeometry> pnt = DrPnt<DrGeometry>(NULL);
    double dist = getInsection(ray, pnt, index);
    if (!pnt) {
        return BLACK;
    }
    
    DrVector point = ray.getPoint(dist);
     OpticalProperty prop;
     pnt->getAppearance(point, prop);
     DrVector norm = pnt->getNormal(point);
    if (norm * ray.direction > 0) norm = -norm;
    
    route += (weight + 1) * index;//naive, need to be improved
    
    DrColor color ;
    //int meet_lighter_index = -1;

    for (auto &i: lights){
        double dist_to_light = i->inside(ray);
        if (getSign(dist_to_light) > 0 && dist > dist_to_light){
            //和光源直接相交，则直接返回光源颜色
            return i->color * i->intensity;
        }
        if (i->lightertype() == 0){// 点光源
            DrVector l_to_p = point - i->position;
            double maxdis = l_to_p.modulus();
            l_to_p.normalize();
            
            DrVector pos = i->position;
            DrRay lray = DrRay(pos, l_to_p);
            if (testShadow(lray, maxdis, pnt)){
                continue;
            }
            
            color += DrPhongShader::get_shade(point, prop, norm, ray, *i, m_ambient) * weight;
        }
        else //矩形光源
        {
            int number = 6;
            DrVector * points = i->getPoints();
            DrVector pntRight = (points[0] - i->position) * (1.0 / number), pntDown = (points[1] - i->position) * (1.0 / number);
            DrColor temp;

            double fra_shade = 1;
            for ( int x = 0 ; x < number ; x++ )
                for ( int y = 0 ; y < number ; y++ ) {
                    DrVector aim = i->position; //aim是阴影射线在光源一端的坐标
                    aim = aim + pntRight * ( (double)rand() / RAND_MAX + x );
                    aim = aim + pntDown * ( (double)rand() / RAND_MAX + y );
                    
                    DrLighter temp_lighter = DrLighter(aim, i->color, i->intensity );
                    DrVector l_to_p = point - aim;
                    double maxdis = l_to_p.modulus();
                    l_to_p.normalize();

                    DrRay lray = DrRay(temp_lighter.position, l_to_p);
                    if (testShadow(lray, maxdis, pnt)){
                        fra_shade -= 1 / pow(number, 2); //这条阴影射线占的比例
                        continue;
                    }
                    temp += DrPhongShader::get_shade(point, prop, norm, ray, temp_lighter, m_ambient)* (1 / pow(number, 2));
                }
            
            color += temp * fra_shade * weight;
        }
    }
    
    if (prop.reflection > 0 && pnt->getRef()){
        //漫反射 随机
        int drefl_sample = 8;
        double drefl = pnt->refrand; //这是决定射线范围椎体“肥瘦”的变量，如果值为0，就是镜面反射了
        if ( drefl > DrEPS && depth <= 1 ) { //只处理drelf不为0，且递归层数为第一层的情况
            double refl = prop.reflection / drefl_sample;
            DrVector RP = -ray.direction.reflection(-norm);
            DrVector RN1 = DrVector( RP.z , RP.y , -RP.x ); //RN1、RN2和RP相互垂直
            DrVector RN2 = RP.cross( RN1 );
            for (int k = 0; k < drefl_sample; ++k){//用drefl_sample条反射射线取平均进行渲染
                double xof, yof;
                do{
                    xof = ( (double) rand() / RAND_MAX) * drefl;
                    yof = ( (double) rand() / RAND_MAX) * drefl;
                }
                while ( sqr( xof ) + sqr( yof ) > sqr( drefl ) );
                DrVector R = RP + RN1 * xof + RN2 * yof;//找到一条符合条件的漫反射射线
                R.normalize();
                color += doRayTracing(DrRay(point, R), weight * refl, 1 + depth, route) * refl * weight;
            }
        }
        else {
           DrVector ref = -ray.direction.reflection(-norm);
           color += doRayTracing(DrRay(point, ref),
                                         weight * prop.reflection, 1+depth, route) * prop.reflection * weight;
        }
   }

    if (prop.transparency){
        DrVector trans;
        if (pnt->getRefraction(trans, point, ray.direction, pnt->rayInside(ray))){
            DrRay frecRay = DrRay(point,trans);
            //Beer Law
            double dist = pnt->intersection(frecRay); //计算光走过的距离
            DrColor absor = pnt->getAbsorb() * 0.15 * -dist; //一个参数，与物体对光的吸收和路径长度的相反数呈正比
            DrColor trans = DrColor( exp( absor.r ) , exp( absor.g ) , exp( absor.b ) ); //用指数函数，计算能通过的光的比例
            
            color += doRayTracing(frecRay, weight * prop.transparency, depth + 1, route) * prop.transparency * trans * weight;
        }
    }
    color.setToRange();

    return color;
}

bool DrScene::testShadow(const DrRay &ray, double max_dist, const DrPnt<DrGeometry> &ii){
    for (const auto &i: objs){
        if (i->intersect(ray)){
            double dis = i->intersection(ray);
            if (getSign(dis - max_dist) < 0){
                if (i != ii){
                    return true;
                }
            }
        }
    }
    return false;
}

double DrScene::getInsection(const DrRay &ray, DrPnt<DrGeometry> &pnt, int& idx){
    double mindis = std::numeric_limits<double>::max();
    int index = 0;
    for (const auto &i: objs){
        index++;
        if (i->intersect(ray)){
            double dis = i->intersection(ray);
            if (dis < mindis){
                mindis = dis;
                pnt = i;
                idx = index;
            }
        }
    }
    return mindis;
}
