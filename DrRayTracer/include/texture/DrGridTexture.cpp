//
//  DrGridTexture.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/24.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrGridTexture.h"
void DrGridTexture::getProperty(const DrVector& vec, OpticalProperty &property){
    double x = fabs(fmod(vec.x, m_width));
    double y = fabs(fmod(vec.y, m_length));
    
    if ((x <= m_border_width || x >= m_width - m_border_width) ||
        (y <= m_border_width || y >= m_length - m_border_width))
        property = grid_line;
    else
        property = grid_space;
}
