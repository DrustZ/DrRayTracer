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
    DrSphere(DrVector& cen, double r, DrPnt<DrTexture>& tex);
    
    bool inside(DrVector& vec);
    
    bool onface(DrVector& vec);
    
    bool intersect(DrRay& ray);
    
    double intersection(DrRay& ray);
    
    DrVector getNormal(DrVector &v);
    
    void sayMyself() { std::cout << "class : sphere" << std::endl; }
    double getRadius() const
    { return radius; }
    
    void setRadius(double r)
    { radius = r; }
};

#endif /* defined(__DrRayTracer__DrSphere__) */
