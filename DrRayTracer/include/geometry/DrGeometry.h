//
//  DrGeometry.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrGeometry__
#define __DrRayTracer__DrGeometry__

#include "DrPnt.h"
#include "DrRay.h"
#include "DrColor.h"
#include "DrMath.h"
#include "DrTexture.h"
#include <vector>
#include <iostream>

class DrGeometry {
public:
    //class DrRayTask;
    
    DrGeometry(DrPnt<DrTexture> tex);

    virtual ~DrGeometry(){};
    
    virtual DrVector getNormal(DrVector &v) = 0;
    
    virtual bool inside (DrVector& vec) = 0;

    virtual double intersection(DrRay& ray) = 0;
    
    virtual bool intersect(DrRay& ray) = 0;
    
    virtual void sayMyself() { std::cout << "class : geometry" << std::endl; }
    void getAppearance(DrVector& vec, OpticalProperty& property)
    { m_texture->getProperty(vec, property); }
    
protected:
    DrPnt<DrTexture> m_texture;
};

#endif /* defined(__DrRayTracer__DrGeometry__) */
