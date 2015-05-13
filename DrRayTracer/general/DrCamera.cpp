//
//  DrCamera.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/12.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrCamera.h"

DrCamera::DrCamera(CameraConfigure& configure, DrScene &scn):conf(configure),scene(scn){
    threads = 4;
}

void DrCamera::startProcess(cv::Mat& image){
    using std::cout;
    using std::endl;
    Result = image;
    
    //一个二维数组，对应每一个像素，
    //作为超级样本之用（反锯齿，每一个项记录对应像素点发出光线追踪物体的路径，如果相邻像素点追踪路径不同，则需要反锯齿化）
    //路径比较用hash来记录。
    Pixels = new int *[Result.rows];
    Pixels[0] = new int[Result.cols * Result.rows];
    for (int i = 1; i < Result.rows; ++i)
        Pixels[i] = Pixels[i-1] + Result.cols;
    
    for (int i = 0; i < Result.rows; ++i)
        for (int j = 0; j < Result.cols; ++j)
            Pixels[i][j] = 0;
    /**
     *  多线程
     */
    //加锁
    lines = new std::mutex[Result.rows];
    
    for (int i = 0; i < threads; ++i)
        thread_amount[i] = 0;
    
    for (int i = 0; i < threads; ++i){
        if (i != threads - 1)
            std::thread (&DrCamera::rayTracing, this, i).detach();
        else std::thread (&DrCamera::rayTracing, this, i).join();
    }
    
    bool all_finished = false;
    while(!all_finished){
        all_finished = true;
        for (int j = 0; j < threads; ++j){
            if (thread_amount[j] == 0){
                all_finished = false;
            }
        }
        usleep(1000);
    }

    if (!conf.open_alias || conf.open_depth)
        return ;
    
//    for (int i = 0 ; i < Result.rows; ++i){
//        for (int j = 0; j < Result.cols; ++j)
//            std::cout << Pixels[i][j] << " ";
//        std::cout << std::endl;
//    }
//    
    //抗锯齿
    for (int i = 0; i < Result.rows; ++i)
        lines[i].unlock();
    
    for (int i = 0; i < threads; ++i)
        thread_amount[i] = 0;
    
    for (int i = 0; i < threads; ++i){
        if (i != threads - 1)
            std::thread (&DrCamera::antiAlias, this, i).detach();
        else std::thread (&DrCamera::antiAlias, this, i).join();
    }
    
    while(!all_finished){
        all_finished = true;
        for (int j = 0; j < threads; ++j)
            if (thread_amount[j] == 0){
                all_finished = false;
            }
        usleep(1000);
    }
    delete Pixels;
}

void DrCamera::rayTracing(int thread_index){
    double eye_to_pixel;
    int tempint;
    
    for (int j = 0; j < Result.rows; ++j){
        if (lines[j].try_lock()){
            for (int i = 0; i < Result.cols; ++i){
                DrColor ret = BLACK;
                DrRay ray = transformToGlobal(i, j, conf.eye_to_img, Result.cols, Result.rows, conf.eyeleft, conf.eyeright, conf.eyeup, conf.eyedown, eye_to_pixel);
                //如果开启景深效果
                if (conf.open_depth){
                    //用相似三角形计算焦平面上的点
                    DrVector focus_point = eye + ray.direction * ( eye_to_pixel *
                                                                  ((conf.eye_to_img + conf.focus_dist) / conf.eye_to_img - 1) );
                    //发出很多聚焦光线
                    int focus_time = 50;
                    for (int k = 1; k <= focus_time; ++k){
                        double angle = (double)rand() / RAND_MAX * 2 * PI;
                        double rand_range = conf.aperture_range * rand() / RAND_MAX;
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
                } else {
                    ret = scene.doRayTracing(ray, 1, 0, Pixels[j][i]);
                    Result.at<cv::Vec3f>(j,i)[0] = ret.b;
                    Result.at<cv::Vec3f>(j,i)[1] = ret.g;
                    Result.at<cv::Vec3f>(j,i)[2] = ret.r;
                }
            }
        }
    }
    thread_amount[thread_index] = 1;
}

void DrCamera::antiAlias(int thread_index){
    int anti_ray_number = 3;
    double tempdouble;
    for (int j = 0; j < Result.rows-3; ++j){
        if (lines[j].try_lock()){
            for (int i = 0; i < Result.cols-3; ++i){
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
                        DrRay ray = transformToGlobal(I, J, conf.eye_to_img, Result.cols, Result.rows, conf.eyeleft, conf.eyeright, conf.eyeup, conf.eyedown, tempdouble);
                        int t = 0;
                        tmp += scene.doRayTracing(ray, 1, 0, t);
                    }
                Result.at<cv::Vec3f>(j,i)[0] = tmp.b / (anti_ray_number * anti_ray_number);
                Result.at<cv::Vec3f>(j,i)[1] = tmp.g / (anti_ray_number * anti_ray_number);
                Result.at<cv::Vec3f>(j,i)[2] = tmp.r / (anti_ray_number * anti_ray_number);
            }
        }
    }
    thread_amount[thread_index] = 1;
}

void DrCamera::getEyePosition(DrVector &e, DrVector &lookat, DrVector &up){
    w = lookat.getNormalize();
    u = up.cross(w).getNormalize();
    v = w.cross(u);
    eye = e;
}

DrRay DrCamera::transformToGlobal(double x, double y, double height,
      int nx, int ny, double left, double right, double up, double down, double& dist){
    
    double us = left + (right - left) * (x + 0.5) / nx;
    double vs = down + (up - down) * (y + 0.5) / ny;
    DrVector vec = u * us + v * vs + w * height;
    dist = vec.modulus();
    return DrRay(eye, vec);
}


