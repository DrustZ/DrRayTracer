//
//  main.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//
#include <thread>
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
int tempint;
double tempdouble;
struct threadPara{
    int col_start;
    int col_end;
    int row_start;
    int row_end;
    threadPara(int a, int b, int c, int d):col_start(a), col_end(b), row_start(a), row_end(b){}
    threadPara(){}
};

//void workThread(threadPara para);
void addObjects();
void addLighters();
void showAndSave();
void antiAlias(double eye_to_img, double eyeup, double eyedown, double eyeleft, double eyeright);

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
    
    addObjects();
    // // // // // //
    addLighters();
    
    DrVector eye = DrVector(40,-500,0);
    DrVector lookat = DrVector(0,1,0);
    DrVector up = DrVector(0,0,-1);
    scene.getEyePosition(eye, lookat, up);
    double eye_to_img = 500,
           eyeup = 100,
           eyedown = -300,
           eyeleft = -200,
            eyeright = 200;
    
    {
//    
//    threadPara para[36];
//    for (int i = 0; i < 6; ++i)
//        for (int j = 0; j < 6; ++j)
//        {
//            para[i * 6 + j] = threadPara(i * 100, i * 100 + 100, j * 100, j * 100 + 100);
//        }
//   
//    std::thread th0(workThread, para[0]);
//    std::thread th1(workThread, para[1]);
//    std::thread th2(workThread, para[2]);
//    std::thread th3(workThread, para[3]);
//    std::thread th4(workThread, para[4]);
//    std::thread th5(workThread, para[5]);
//    std::thread th6(workThread, para[6]);
//    std::thread th7(workThread, para[7]);
//    std::thread th8(workThread, para[8]);
//    std::thread th9(workThread, para[9]);
//    std::thread th10(workThread, para[10]);
//    std::thread th11(workThread, para[11]);
//    std::thread th12(workThread, para[12]);
//    std::thread th13(workThread, para[13]);
//    std::thread th14(workThread, para[14]);
//    std::thread th15(workThread, para[15]);
//    std::thread th16(workThread, para[16]);
//    std::thread th17(workThread, para[17]);
//    std::thread th18(workThread, para[18]);
//    std::thread th19(workThread, para[19]);
//    std::thread th20(workThread, para[20]);
//    std::thread th21(workThread, para[21]);
//    std::thread th22(workThread, para[22]);
//    std::thread th23(workThread, para[23]);
//    std::thread th24(workThread, para[24]);
//    std::thread th25(workThread, para[25]);
//    std::thread th26(workThread, para[26]);
//    std::thread th27(workThread, para[27]);
//    std::thread th28(workThread, para[28]);
//    std::thread th29(workThread, para[29]);
//    std::thread th30(workThread, para[30]);
//    std::thread th31(workThread, para[31]);
//    std::thread th32(workThread, para[32]);
//    std::thread th33(workThread, para[33]);
//    std::thread th34(workThread, para[34]);
//    std::thread th35(workThread, para[35]);
//
//
//        th0.join();
//        th1.join();
//        th2.join();
//        th3.join();
//        th4.join();
//        th5.join();
//        th6.join();
//        th7.join();
//        th8.join();
//        th9.join();
//        th10.join();
//        th11.join();
//    th12.join();
//    th13.join();
//    th14.join();
//    th15.join();
//    th16.join();
//    th17.join();
//    th18.join();
//    th19.join();
//    th20.join();
//    th21.join();
//    th22.join();
//    th23.join();
//    th24.join();
//    th25.join();
//    th26.join();
//    th27.join();
//    th28.join();
//    th29.join();
//    th30.join();
//    th31.join();
//    th32.join();
//    th33.join();
//    th34.join();
//    th35.join();
//    
    }
    for (int i = 0; i < Result.cols; ++i){
        for (int j = 0; j < Result.rows; ++j){
            DrColor ret = BLACK;
            
            double focus_dist = 10;//800;
            double eye_to_pixel = 0;
            //目前是x z 轴为平面的圆孔，需要重新写相机类来重构
            double aperture_range = 0;//15;
            DrRay ray = scene.transformToGlobal(i, j, eye_to_img, Result.cols, Result.rows, eyeleft, eyeright, eyeup, eyedown, eye_to_pixel);
            //用相似三角形计算焦平面上的点
            DrVector focus_point = eye + ray.direction * ( eye_to_pixel *
                                        ((eye_to_img + focus_dist) / eye_to_img - 1) );
//            DrVector focus_point = eye + ray.direction * focus_dist;
//            cout << focus_point;
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
//            DrVector st = DrVector(i, 0, j);
//            DrRay ray = DrRay(st, lookat);
            Result.at<cv::Vec3f>(j,i)[0] = ret.b / focus_time;
            Result.at<cv::Vec3f>(j,i)[1] = ret.g / focus_time;
            Result.at<cv::Vec3f>(j,i)[2] = ret.r / focus_time;
        }
    }
    
    antiAlias(eye_to_img, eyeup, eyedown, eyeleft, eyeright);
    
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
        scene.addlights(lighter2);
//    scene.addlights(lighter_rect);

}

void antiAlias(double eye_to_img, double eyeup, double eyedown, double eyeleft, double eyeright){
    
    int anti_ray_number = 3;
    for (int i = 0; i < Result.cols; ++i)
        for (int j = 0; j < Result.rows; ++j){
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
//
//void workThread(threadPara para){
//    int col_start = para.col_start;
//    int col_end = para.col_end;
//    int row_start = para.row_start;
//    int row_end = para.row_end;
//    for (int i = col_start; i < col_end; ++i){
//        for (int j = row_start; j < row_end; ++j){
//            DrRay ray = scene.transformToGlobal(i, j, 500, Result.cols, Result.rows, -200, 200, 100, -300);
//            DrColor ret = scene.doRayTracing(ray, 1, 0, Pixels[j][i]);
//            Result.at<cv::Vec3f>(j,i)[0] = ret.b;
//            Result.at<cv::Vec3f>(j,i)[1] = ret.g;
//            Result.at<cv::Vec3f>(j,i)[2] = ret.r;
//        }
//    }
//    
//    int anti_ray_number = 3;
//    for (int i = 0; i < Result.cols; ++i)
//        for (int j = 0; j < Result.rows; ++j){
//            bool check = false;
//            for ( int a = -1 ; a <= 1 && !check ; a++ )
//                for ( int b = -1 ; b <= 1 && !check ; b++ ) {
//                    if ( i + a < 0 || a + i >= Result.cols ) continue;
//                    if ( j + b < 0 || j + b >= Result.rows ) continue;
//                    if ( Pixels[j][i] != Pixels[j+b][i+a] ) check = true;
//                }
//            if ( !check ) continue; //判断是否需要使用超级样本
//            
//            DrColor tmp = BLACK;
//            for ( int a = 0 ; a < anti_ray_number ; a++ )
//                for ( int b = 0 ; b < anti_ray_number ; b++ ) {
//                    double I = ( double ) i - 0.5 + ( 0.5 + a ) / anti_ray_number;
//                    double J = ( double ) j - 0.5 + ( 0.5 + b ) / anti_ray_number;
//                    DrRay ray = scene.transformToGlobal(I, J, 500, Result.cols, Result.rows, -200, 200, 100, -300);
//                    int t = 0;
//                    tmp += scene.doRayTracing(ray, 1, 0, t);
//                }
//            Result.at<cv::Vec3f>(j,i)[0] = tmp.b / (anti_ray_number * anti_ray_number);
//            Result.at<cv::Vec3f>(j,i)[1] = tmp.g / (anti_ray_number * anti_ray_number);
//            Result.at<cv::Vec3f>(j,i)[2] = tmp.r / (anti_ray_number * anti_ray_number);
//        }
//}
