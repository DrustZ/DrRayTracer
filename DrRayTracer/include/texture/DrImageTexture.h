//
//  DrImageTexture.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/25.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrImageTexture__
#define __DrRayTracer__DrImageTexture__

#include "DrTexture.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class DrImageTexture : public DrTexture {
    OpticalProperty m_base;
    double x_scale, y_scale;
    cv::Mat image;
    
public:
    DrImageTexture(const char *path,
                   const OpticalProperty &base,
                   double xs, double ys);
    
    int getClass()
    { return -1; }
    
    void getProperty(const DrVector& vec, OpticalProperty &property);
};
#endif /* defined(__DrRayTracer__DrImageTexture__) */
