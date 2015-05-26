//
//  main.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//
#include <thread>
#include <time.h>
#include <mutex>
#include <unistd.h>
#include <iostream>
#include "DrCamera.h"
#include "DrScene.h"
#include "DrUniformTexture.h"
#include "DrGridTexture.h"
#include "DrWoodenTexture.h"
#include "DrImageTexture.h"
#include "DrLighter.h"
#include "DrRectLighter.h"
#include "DrTriangle.h"
#include "DrObjReader.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

DrScene scene = DrScene(4, 0.01, BLACK);
cv::Mat Result = cv::Mat::zeros(600, 600, CV_32FC3);
int tempint;
double tempdouble;

DrVector eye = DrVector(0,-500,200);
DrVector lookat = DrVector(0,100,-50);
DrVector up = DrVector(0,0,-1);

void workThread(int index);
void addObjects();
void addLighters();
void showAndSave();
void antiAlias(int index);
int main() {
    using std::cout;
    using std::endl;
    using std::string;
    
    addLighters();
//    addObjects();
    DrObjReader reader("/Users/zmr/codes/c_c++/Graphics/rayTracerSources/cornell-box/CornellBox-Empty-White.obj");
    if(reader.read()) reader.load(scene);
    
    DrCamera::CameraConfigure conf;
    conf.open_alias = true;
    DrCamera camera(conf, scene);
    camera.getEyePosition(eye, lookat, up);
    camera.startProcess(Result);
    showAndSave();

    return 0;
}

/*
 *添加物体
 */
void addObjects()
{
        OpticalProperty prop;
        prop.diffuse = BLACK;
        prop.specular = 0;//DrColor(0.3,0.3,0.3);
        prop.spec_exp = 222;
        prop.transparency = 0;
        prop.reflection = 0.0;
        // prop.ambient = 0.1;
        
        DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/dots.png", prop, 2, 2));

        DrVector dv = DrVector(-100,120,50);
        DrPnt<DrGeometry> ball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, true, 0, 1.3));
        ball->setAbsorb(DrColor(0,0.4,0.4));
        //scene.addObj(ball);
        
        prop.diffuse = DrColor(0.5,0.2,0.2);
        prop.specular = 0.3;
        prop.spec_exp = 80;
        prop.reflection = 0;
        prop.transparency = 0;
        texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
        dv = DrVector(50,80,200);
        DrPnt<DrGeometry> ball2 = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0, 1.5));
//        scene.addObj(ball2);
    dv = DrVector(100,120,150);
    DrPnt<DrGeometry> aball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0, 1.5));
    scene.addObj(aball);
    dv = DrVector(0,120,60);
    prop.reflection = 0;
    //texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/marble2.jpg", prop, 1, 1));
    texture = DrPnt<DrTexture>(new DrUniformTexture(prop));

    DrPnt<DrGeometry> bball = DrPnt<DrGeometry>(new DrSphere(dv, 60, texture, true, 0, 1.5));
    scene.addObj(bball);
    prop.reflection = 0;

    texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/wood.jpg", prop, 1, 1));
    dv = DrVector(-20,-40,100);
        DrPnt<DrGeometry> cball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0, 1.5));
    scene.addObj(cball);
    prop.transparency = 0.2;

    texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/bmqk.jpg", prop, 1, 1));
    dv = DrVector(-50,-80,70);
        DrPnt<DrGeometry> dball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0, 1.2));
    scene.addObj(dball);
    
        dv = DrVector(160, 80, 80);
        prop.diffuse = DrColor(0.2, 0.3, 0.4);
        prop.reflection = 0.5;
        prop.transparency = 0.1;
        texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
        DrPnt<DrGeometry> ball3 = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, true, 0 , 1.33));
        scene.addObj(ball3);
    
        prop.diffuse = DrColor(0.8,0.8,0.8);
        prop.spec_exp = 300;
        prop.specular = 0.1;//DrColor(0.2,0.2,0.2);
        prop.reflection = 0.4;
        OpticalProperty prop_space;
        prop_space.diffuse = DrColor(0.2,0.1,0.4);
        prop_space.specular = 0.1;//DrColor(0.3,0.3,0.2);
        prop_space.spec_exp = 111;
        prop_space.transparency = 0;
        prop_space.reflection = 0.2;
        DrPnt<DrTexture> texture_grid = DrPnt<DrTexture>(new DrGridTexture(prop_space, prop));
        dv = DrVector(0,0,1).getNormalize();
        
        prop_space.diffuse = DrColor(0.83, 0.6, 0.25);
        prop.diffuse = BLACK;//DrColor(0.2,0.82,0);
        prop.reflection = 0.8;
        prop.transparency = 0;
        
        prop.diffuse = DrColor(0.5, 0.5, 0.5);
        texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/wood.jpg", prop, 3, 3));
    
        DrPnt<DrGeometry> planeB = DrPnt<DrGeometry>(new DrPlane(dv, 0, texture, true, 0, 1.2));
//        scene.addObj(planeB);
        prop.diffuse = DrColor(0.6, 0.6, 0.6);
        prop.reflection = 0;
        prop.transparency = 0.6;
        texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
//
        dv = DrVector(0,1,0).getNormalize();
//        texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/beach.jpg", prop, 1, 0.9));
        DrPnt<DrGeometry> planeC = DrPnt<DrGeometry>(new DrPlane(dv, 00, texture, false, 0, 1));
//        scene.addObj(planeC);
        DrPnt<DrGeometry> triangle = DrPnt<DrGeometry>(new DrTriangle(DrVector(-50,0,0), DrVector(150,30,0), DrVector(30,0,200), texture, true, 0, 1.2));
        scene.addObj(triangle);
    
}

/*
 *添加光源
 */
void addLighters()
{
    DrVector dv2 = DrVector(0,-40,200);
    DrPnt<DrLighter> lighter = DrPnt<DrLighter>(new DrLighter(dv2, WHITE, 0.7));
    dv2 = DrVector(50, -200, 50);
    DrPnt<DrLighter> lighter2 = DrPnt<DrLighter>(new DrLighter(dv2, WHITE, 0.8));
    DrVector dvright = DrVector(20, 100, 200);
    DrVector dvdown = DrVector(-20, 60, 200);
    
    DrPnt<DrLighter> lighter_rect = DrPnt<DrLighter>(new DrRectLighter(dv2, WHITE, 0.8, dvright, dvdown));
    scene.addlights(lighter);
        scene.addlights(lighter2);
//    scene.addlights(lighter_rect);

}

void showAndSave()
{
    
    cv::Mat verseImage = cv::Mat::zeros(Result.rows, Result.cols, Result.type());
    for (int i = 0; i < Result.rows; ++i)
        Result.row(Result.rows - 1 - i).copyTo(verseImage.row(i));
    cv::Mat image = cv::Mat::zeros(Result.rows, Result.cols, CV_8UC3);
    Result.convertTo(image, CV_8UC3, 255);
    imwrite( "/Users/zmr/codes/c_c++/Graphics/DrRayTracer/image.bmp", image);
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", Result);
    
    cv::waitKey(0);
}
