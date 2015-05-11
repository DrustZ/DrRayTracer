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
#include "include/DrScene.h"
#include "include/texture/DrUniformTexture.h"
#include "include/texture/DrGridTexture.h"
#include "include/texture/DrWoodenTexture.h"
#include "include/texture/DrImageTexture.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

DrScene scene = DrScene(4, 0.01, BLACK);
cv::Mat Result = cv::Mat::zeros(600, 600, CV_32FC3);
int ** Pixels = new int*[Result.rows];
std::mutex * lines;
int tempint,thread_amount[9];
double tempdouble;

/**
 *  景深的参数 & 相机的参数
 */

DrVector eye = DrVector(40,-500,0);
DrVector lookat = DrVector(0,1,0);
DrVector up = DrVector(0,0,-1);
double eye_to_img = 500,
eyeup = 100,
eyedown = -300,
eyeleft = -200,
eyeright = 200;
double focus_dist = 10;//800;
double eye_to_pixel = 0;
//目前是x z 轴为平面的圆孔，需要重新写相机类来重构
double aperture_range = 0;//15;

void workThread(int index);
void addObjects();
void addLighters();
void showAndSave();
void antiAlias(int index);
void say(){ std::cout << "aaa"; }
int main() {
    using std::cout;
    using std::endl;
    using std::string;
    
    //一个二维数组，对应每一个像素，
    //作为超级样本之用（反锯齿，每一个项记录对应像素点发出光线追踪物体的路径，如果相邻像素点追踪路径不同，则需要反锯齿化）
    //路径比较用hash来记录。
    Pixels[0] = new int[Result.cols * Result.rows];
    for (int i = 1; i < Result.rows; ++i)
        Pixels[i] = Pixels[i-1] + Result.cols;
    
    clock_t start,finish;
    start = clock();
    
    addObjects();
    addLighters();
    scene.getEyePosition(eye, lookat, up);
    /**
     *  多线程
     */
    //加锁
    lines = new std::mutex[Result.rows];
    
    int threads = 4;
    for (int i = 0; i < threads; ++i)
        thread_amount[i] = 0;
    
    for (int i = 0; i < threads; ++i){
        if (i != threads - 1)
            std::thread (workThread, i).detach();
        else std::thread (workThread, i).join();
    }
    
    bool all_finished = false;
    while(!all_finished){
        all_finished = true;
        for (int j = 0; j < threads; ++j)
            if (thread_amount[j] == 0){
                all_finished = false;
            }
        usleep(1000);
    }
    
    for (int i = 0; i < Result.rows; ++i)
        lines[i].unlock();
    
    for (int i = 0; i < threads; ++i)
        thread_amount[i] = 0;
    
    for (int i = 0; i < threads; ++i){
        if (i != threads - 1)
            std::thread (antiAlias,i).detach();
        else std::thread (antiAlias, i).join();
    }
    
    while(!all_finished){
        all_finished = true;
        for (int j = 0; j < threads; ++j)
            if (thread_amount[j] == 0){
                all_finished = false;
            }
        usleep(1000);
    }

    finish = clock();
    cout << static_cast<double>(finish - start) / CLOCKS_PER_SEC * 1000;

    showAndSave();

    delete Pixels;
    return 0;
}

void addObjects()
{
        OpticalProperty prop;
        prop.diffuse = BLACK;
        prop.specular = 0;//DrColor(0.3,0.3,0.3);
        prop.spec_exp = 222;
        prop.transparency = 1;
        prop.reflection = 0.01;
        // prop.ambient = 0.1;
        
        DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/dots.png", prop, 1, 1));

        DrVector dv = DrVector(-60,120,200);
        DrPnt<DrGeometry> ball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, true, 0.8, 1.3));
        ball->setAbsorb(DrColor(0,0.4,0.4));
        scene.addObj(ball);
        
        prop.diffuse = DrColor(0.5,0.8,0.4);
        prop.specular = 0.6;
        prop.spec_exp = 50;
        prop.reflection = 0;
        prop.transparency = 0;
        texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
        dv = DrVector(50,80,200);
        DrPnt<DrGeometry> ball2 = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0.8, 1.5));
//        scene.addObj(ball2);
    dv = DrVector(100,120,150);
    DrPnt<DrGeometry> aball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0.8, 1.5));
//    scene.addObj(aball);
    dv = DrVector(20,60,120);
    prop.reflection = 0.2;
    texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/marble2.jpg", prop, 1, 1));
    DrPnt<DrGeometry> bball = DrPnt<DrGeometry>(new DrSphere(dv, 60, texture, false, 0.8, 1.5));
    scene.addObj(bball);
    prop.reflection = 0;

    texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/wood.jpg", prop, 1, 1));
    dv = DrVector(-20,-40,100);
        DrPnt<DrGeometry> cball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0.8, 1.5));
//    scene.addObj(cball);
    prop.transparency = 0.2;

    texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/bmqk.jpg", prop, 1, 1));
    dv = DrVector(-50,-80,70);
        DrPnt<DrGeometry> dball = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, false, 0.8, 1.2));
    scene.addObj(dball);
    
        
        dv = DrVector(160, 20, 100);
        prop.diffuse = BLACK;//(0.2, 0.6, 0.4);
        prop.reflection = 1;
        prop.transparency = 0.1;
        texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
        DrPnt<DrGeometry> ball3 = DrPnt<DrGeometry>(new DrSphere(dv, 50, texture, true, 1.2, 1.33));
        scene.addObj(ball3);
        
        prop.diffuse = DrColor(0.8,0.8,0.8);
        prop.spec_exp = 300;
        prop.specular = 0.5;//DrColor(0.2,0.2,0.2);
        prop.reflection = 0.3;
        OpticalProperty prop_space;
        prop_space.diffuse = DrColor(0.2,0.1,0.4);
        prop_space.specular = 0.5;//DrColor(0.3,0.3,0.2);
        prop_space.spec_exp = 300;
        prop_space.transparency = 0.1;
        prop_space.reflection = 0.3;
        DrPnt<DrTexture> texture_grid = DrPnt<DrTexture>(new DrGridTexture(prop_space, prop));
        dv = DrVector(0,-10,20).getNormalize();
        
        prop_space.diffuse = DrColor(0.83, 0.6, 0.25);
        prop.diffuse = DrColor(0.2,0.82,0);
        prop.reflection = 0.5;
        prop.transparency = 0.2;
        
        prop.diffuse = DrColor(0.5, 0.5, 0.5);
        texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/marble3", prop, 3, 3));
    
        DrPnt<DrGeometry> planeB = DrPnt<DrGeometry>(new DrPlane(dv, -10, texture_grid, true, 0.7, 1.2));
        scene.addObj(planeB);
        prop.diffuse = DrColor(0.7, 0.2, 0);
        prop.reflection = 0;
        prop.transparency = 0;
        texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
        
        dv = DrVector(0,-10,0).getNormalize();
        texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/beach.jpg", prop, 1, 0.9));
        DrPnt<DrGeometry> planeC = DrPnt<DrGeometry>(new DrPlane(dv, -300, texture, true, 0.8, 1.2));
        scene.addObj(planeC);
        
}

void addLighters()
{
    DrVector dv2 = DrVector(-30,-200,400);
    DrPnt<DrLighter> lighter = DrPnt<DrLighter>(new DrLighter(dv2, WHITE, 1));
    dv2 = DrVector(80, 40, 250);
    DrPnt<DrLighter> lighter2 = DrPnt<DrLighter>(new DrLighter(dv2, WHITE, 1));
    DrVector dvright = DrVector(130, 40, 250);
    DrVector dvdown = DrVector(80, 0, 250);
    
    DrPnt<DrLighter> lighter_rect = DrPnt<DrLighter>(new DrRectLighter(dv2, WHITE, 0.9, dvright, dvdown));
//    scene.addlights(lighter);
//        scene.addlights(lighter2);
    scene.addlights(lighter_rect);

}

void antiAlias(int index){
    int anti_ray_number = 3;
    for (int j = 0; j < Result.rows; ++j){
        if (lines[j].try_lock()){
            for (int i = 0; i < Result.cols; ++i){
                bool check = false;
                for ( int a = -1 ; a <= 1 && !check ; a++ )
                    for ( int b = -1 ; b <= 1 && !check ; b++ ) {
                        if ( i + a < 0 || a + i >= Result.cols ) continue;
                        if ( j + b < 0 || j + b >= Result.rows ) continue;
                        if ( Pixels[j][i] != Pixels[j+b][i+a] ) check = true;
                    }
                if ( !check ) continue; //判断是否需要使用超级样本
                
                DrColor tmp = BLACK;
                for ( int a = 0 ; a < anti_ray_number ; a++ )
                    for ( int b = 0 ; b < anti_ray_number ; b++ ) {
                        double I = ( double ) i - 0.5 + ( 0.5 + a ) / anti_ray_number;
                        double J = ( double ) j - 0.5 + ( 0.5 + b ) / anti_ray_number;
                        DrRay ray = scene.transformToGlobal(I, J, eye_to_img, Result.cols, Result.rows, eyeleft, eyeright, eyeup, eyedown, tempdouble);
                        int t = 0;
                        tmp += scene.doRayTracing(ray, 1, 0, t);
                    }
                Result.at<cv::Vec3f>(j,i)[0] = tmp.b / (anti_ray_number * anti_ray_number);
                Result.at<cv::Vec3f>(j,i)[1] = tmp.g / (anti_ray_number * anti_ray_number);
                Result.at<cv::Vec3f>(j,i)[2] = tmp.r / (anti_ray_number * anti_ray_number);
            }
        }
    }
    thread_amount[index] = 1;
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

void workThread(int index){
    for (int j = 0; j < Result.rows; ++j){
        if (lines[j].try_lock()){
            for (int i = 0; i < Result.cols; ++i){
                    DrColor ret = BLACK;
                    DrRay ray = scene.transformToGlobal(i, j, eye_to_img, Result.cols, Result.rows, eyeleft, eyeright, eyeup, eyedown, eye_to_pixel);
                    //用相似三角形计算焦平面上的点
                    DrVector focus_point = eye + ray.direction * ( eye_to_pixel *
                                                                  ((eye_to_img + focus_dist) / eye_to_img - 1) );
                    int focus_time = 1;
                    for (int k = 1; k <= focus_time; ++k){
                        double angle = (double)rand() / RAND_MAX * 2 * PI;
                        double rand_range = aperture_range * rand() / RAND_MAX;
                        DrVector rand_eye = DrVector(eye.x + rand_range * cos(angle), eye.y, eye.z + rand_range * sin(angle));
                        
                        DrVector dir = focus_point - rand_eye;
                        DrRay rand_ray = DrRay(rand_eye, dir);
                        if (k == 1)
                            ret += scene.doRayTracing(rand_ray, 1, 0, Pixels[j][i]);
                        else{
                            tempint = 0;
                            ret += scene.doRayTracing(rand_ray, 1, 0, tempint);
                        }
                    }
                    Result.at<cv::Vec3f>(j,i)[0] = ret.b / focus_time;
                    Result.at<cv::Vec3f>(j,i)[1] = ret.g / focus_time;
                    Result.at<cv::Vec3f>(j,i)[2] = ret.r / focus_time;
                }
        }
    }
    thread_amount[index] = 1;
}
