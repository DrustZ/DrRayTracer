//
//  DrImageTexture.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/25.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrImageTexture.h"
using namespace cv;

DrImageTexture::DrImageTexture(const char *path, const OpticalProperty &base,
                               double xs, double ys) :
                m_base(base), x_scale(xs), y_scale(ys){
    image = imread(path, CV_LOAD_IMAGE_COLOR);   // Read the file
    image.convertTo(image, CV_32FC3, 1/255.0);
}

void DrImageTexture::getProperty(const DrVector& vec, OpticalProperty &property){
    double x = vec.x * x_scale;
    double y = vec.y * y_scale;
    x = fmod(x, 1);
    y = fmod(y, 1);
    if (x < 0) x += 1;
    if (y < 0) y += 1;

    OpticalProperty ret = OpticalProperty(m_base);
    double r = image.at<Vec3f>((int)(y * image.rows),(int)(x * image.cols))[2];
    double g = image.at<Vec3f>((int)(y * image.rows),(int)(x * image.cols))[1];
    double b = image.at<Vec3f>((int)(y * image.rows),(int)(x * image.cols))[0];
    ret.diffuse = DrColor(r, g, b);//*(1 - ret.transparency);
    property = ret;
}
