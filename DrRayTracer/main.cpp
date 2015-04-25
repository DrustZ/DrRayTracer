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
#include "DrGridTexture.h"
#include "DrWoodenTexture.h"
#include "DrImageTexture.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main() {
    using std::cout;
    using std::endl;
    cv::Mat Result = cv::Mat::zeros(600, 600, CV_32FC3);
    DrScene scene = DrScene(3, 0.01, BLACK);
    OpticalProperty prop;
    prop.diffuse = DrColor(0,0.4,0.8);
    prop.specular = 0.2;
    prop.spec_exp = 1111;
    prop.transparency = 0.2;
   // prop.ambient = 0.1;
    
    DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
    DrVector dv = DrVector(100,60,150);
    DrPnt<DrGeometry> ball = DrPnt<DrGeometry>(new DrSphere(dv, 40, texture, true, 1.5));
    scene.addObj(ball);
    
    prop.diffuse = DrColor(1,0,0.6);
    prop.specular = 0.5;
    texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
    dv = DrVector(300,200,300);
    DrPnt<DrGeometry> ball2 = DrPnt<DrGeometry>(new DrSphere(dv, 150, texture, true, 1.5));
    scene.addObj(ball2);
    
    prop.diffuse = WHITE;//DrColor(0.8,0.3,0.6);
    prop.spec_exp = 333;
    prop.specular = 0.3;
    OpticalProperty prop_space;
    prop_space.diffuse = DrColor(0.6,0.4,0.9);
    prop_space.specular = 0.6;
    prop_space.spec_exp = 111;
    prop_space.transparency = 0.02;
    DrPnt<DrTexture> texture_grid = DrPnt<DrTexture>(new DrGridTexture(prop_space, prop));
    dv = DrVector(0,-10,20).getNormalize();
    
    prop_space.diffuse = DrColor(0.83, 0.6, 0.25);
    prop.diffuse = DrColor(0.39,0.22,0);
    prop.specular = 0.1;
    
    DrPnt<DrGeometry> planeB = DrPnt<DrGeometry>(new DrPlane(dv, -10, texture_grid, true, 0));
    scene.addObj(planeB);
    
    texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/codes/Matlab/2.jpg", prop, 2, 2));
    dv = DrVector(40,20,40);
    DrPnt<DrGeometry> ball3 = DrPnt<DrGeometry>(new DrSphere(dv, 60, texture, true, 1.5));
    scene.addObj(ball3);
    
// // // // // //
    DrVector dv2 = DrVector(100,-40,50);
    DrLighter lighter = DrLighter(dv2, WHITE, 0.5);
    dv2 = DrVector(30, -50, 100);
    DrLighter lighter2 = DrLighter(dv2, WHITE, 0.5);
    dv2 = DrVector(40, -100 , 100);
    DrLighter lighter3 = DrLighter(dv2, WHITE, 0.7);
//    scene.addlights(lighter);
    scene.addlights(lighter2);
    scene.addlights(lighter3);

    DrVector eye = DrVector(40,-100,50);
    DrVector lookat = DrVector(0,1,0);
    DrVector up = DrVector(0,0,-1);
    scene.getEyePosition(eye, lookat, up);

    for (int i = 0; i < Result.cols; ++i){
        for (int j = 0; j < Result.rows; ++j){
           // std::cout<<i <<" "<<j<<" " <<std::endl;
            DrRay ray = scene.transformToGlobal(i, j, 100, Result.cols, Result.rows, -200, 200, 200, -200);
//            DrVector st = DrVector(i, 0, j);
//            DrRay ray = DrRay(st, lookat);
            DrColor ret = scene.doRayTracing(ray, 1, 0);
//            cout << ray.direction << endl;
            Result.at<cv::Vec3f>(j,i)[0] = ret.b;
            Result.at<cv::Vec3f>(j,i)[1] = ret.g;
            Result.at<cv::Vec3f>(j,i)[2] = ret.r;
        }
    }
//    DrVector dir = DrVector(eye, dv);
//    cout << dir;
//    std::cout << Result;
    cv::Mat verseImage = cv::Mat::zeros(Result.rows, Result.cols, Result.type());
    for (int i = 0; i < Result.rows; ++i)
        Result.row(Result.rows - 1 - i).copyTo(verseImage.row(i));
    cv::Mat image = cv::Mat::zeros(Result.rows, Result.cols, CV_8UC3);
    Result.convertTo(image, CV_8UC3, 255);
    imwrite( "/Users/zmr/codes/c_c++/Graphics/DrRayTracer/image.bmp", image);
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", Result);
    
    cv::waitKey(0);
    return 0;
}
