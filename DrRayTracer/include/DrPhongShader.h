//
//  DrPhongShader.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrPhongShader__
#define __DrRayTracer__DrPhongShader__
#include "DrMath.h"
#include "DrGeometry.h"
#include "DrLighter.h"
#include <cmath>

/*
 *phong光照模型
 */
class DrPhongShader
{
    
public:
    //pt:物体表面上的点  app表面特性 nrm判断视线与入射光线是否在同一边的向量 warp_nrm表面法向量
    //view_ray 视线
    static DrColor get_shade(DrVector &pt, OpticalProperty &app, DrVector &warp_nrm,
                      const DrRay &ray, DrLighter &lighter, DrColor &ambient_color){

        DrColor ret;

        DrVector point_to_light = DrVector(pt, lighter.position).getNormalize();

        
        //理想漫反射
        double cosin = point_to_light * warp_nrm;
        if (cosin > 0){
            ret += app.diffuse * lighter.color
            * lighter.intensity * cosin;
        }
            
        //镜面反射光
        cosin = -ray.direction * point_to_light.reflection(warp_nrm);
        if (cosin > 0){
            ret += lighter.color * lighter.intensity *
                    app.specular * pow(cosin, app.spec_exp);
        }
        //环境光
//        ret += ambient_color * app.ambient;

//        ret.setToRange();
        

        return ret;
    };
    
};
#endif /* defined(__DrRayTracer__DrPhongShader__) */
