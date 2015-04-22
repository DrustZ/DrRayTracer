//
//  main.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include <iostream>
#include "DrScene.h"
#include "DrUniformTexture.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main() {
    using std::cout;
    using std::endl;
    cv::Mat Result = cv::Mat::zeros(400, 400, CV_32FC3);
    DrColor black = DrColor(0,0,0);
    DrScene scene = DrScene(1, 0.3, black);
    OpticalProperty prop;
    prop.diffuse = DrColor(0,0,0.8);
    prop.specular = 1;
    prop.spec_exp = 4;
    
    DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
    DrVector dv = DrVector(300,200,300);
    DrVector dv2 = DrVector(4400,1900,-1100);
    DrPnt<DrGeometry> ball = DrPnt<DrGeometry>(new DrSphere(dv, 150, texture));
    scene.addObj(ball);
    DrLighter lighter = DrLighter(dv2, WHITE, 1);
    scene.addlights(lighter);
    
    DrVector eye = DrVector(300,0,300);
    DrVector lookat = DrVector(0,1,0);
    DrVector up = DrVector(0,0,1);
    scene.getEyePosition(eye, lookat, up);

    for (int i = 0; i < Result.cols; ++i){
        for (int j = 0; j < Result.rows; ++j){
            DrRay ray = scene.transformToGlobal(i-150, j-150, 100);
            DrColor ret = scene.doRayTracing(ray, 1, 0);
            
            //cout << ray.direction << endl;
            Result.at<cv::Vec3f>(j,i)[0] = ret.b;
            Result.at<cv::Vec3f>(j,i)[1] = ret.g;
            Result.at<cv::Vec3f>(j,i)[2] = ret.r;
        }
    }
    DrVector dir = DrVector(eye, dv);
    cout << dir;
//    std::cout << Result;
    cv::Mat image = cv::Mat::zeros(400, 400, CV_8UC3);
    Result.convertTo(image, CV_8UC3, 255);
    imwrite( "/Users/zmr/codes/c_c++/Graphics/DrRayTracer/image.bmp", image);
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", Result);
    
    cv::waitKey(0);
    return 0;
}
