//
//  DrGridTexture.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrGridTexture__
#define __DrRayTracer__DrGridTexture__

#include "DrTexture.h"

class DrGridTexture : public DrTexture{
    OpticalProperty grid_space, grid_line;
    double m_length, m_width, m_border_width;
    
public:
    DrGridTexture(const OpticalProperty &prpt_space,
                const OpticalProperty &prpt_line,
                double grid_width = 100, double grid_length = 40, double border_width = 5):
    grid_space(prpt_space), grid_line(prpt_line),
    m_width(grid_width), m_length(grid_length), m_border_width(border_width){}
    
    int getClass()
    { return 2; }
    
    void getProperty(const DrVector& vec, OpticalProperty &property);

};

#endif /* defined(__DrRayTracer__DrGridTexture__) */
