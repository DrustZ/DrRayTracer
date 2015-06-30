//
//  DrSphere.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrSphere__
#define __DrRayTracer__DrSphere__

#include "DrGeometry.h"

class DrSphere: public DrGeometry {
    DrVector center;
    double radius;
    
public:
    DrSphere(DrVector& cen, double r, DrPnt<DrTexture>& tex, bool ref, double refr, double trans);
    
    bool inside(const DrVector& vec);
    
    bool rayInside(const DrRay& ray);

    bool onface(const DrVector& vec);
    
    bool intersect(const DrRay& ray);
    
    double intersection(const DrRay& ray);
    
    DrVector getNormal(const DrVector &v);
    
    bool getRefraction(DrVector& refraction, const DrVector& point, 
                       const DrVector& view, bool inside);

    /*
     *获得材质
     */
    void getAppearance(DrVector& vec, OpticalProperty& property);
    
    int sayMyself() { std::cout << "class : sphere" << std::endl; return 2;}
    double getRadius() const
    { return radius; }
    
    void setRadius(double r)
    { radius = r; }
};

#endif /* defined(__DrRayTracer__DrSphere__) */
