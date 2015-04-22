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

class DrPhongShader
{
    
public:
    //pt:物体表面上的点  app表面特性 nrm判断视线与入射光线是否在同一边的向量 warp_nrm表面法向量
    //view_ray 视线
    static DrColor get_shade(DrVector &pt, OpticalProperty &app, DrVector &warp_nrm,
                      DrRay &view_ray,const DrLighter &lighter, DrColor &ambient_color){
        DrVector unit_view, unit_nrm, point_to_light, half_v, nrm;
        DrColor ret;
        double spec_iny;
        
        double ambient = app.ambient,
        specular = app.specular,
        spec_exp = app.spec_exp;
        
        unit_view = view_ray.direction;
        unit_nrm = nrm.getNormalize();
        
        point_to_light = (lighter.position - pt).getNormalize();

        double cosin = fabs(warp_nrm * point_to_light);
        
        // calculate half-way DrVector
        half_v = (point_to_light - unit_view).getNormalize();
        
        // calculate specular intensity
        spec_iny = specular * pow(fabs(warp_nrm * half_v), spec_exp);
        
        ret = ret + app.diffuse * lighter.intensity * cosin * lighter.color;
        
        ret = ret + WHITE * (spec_iny * lighter.intensity) * lighter.color;
        
        //ret = ret + ambient_color * app.ambient;

        return ret;
    };
    
};
#endif /* defined(__DrRayTracer__DrPhongShader__) */
