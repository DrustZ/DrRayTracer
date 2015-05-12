//
//  DrCamera.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/12.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrCamera__
#define __DrRayTracer__DrCamera__

#include <stdio.h>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "DrColor.h"
#include "DrScene.h"
#include "DrRay.h"

class DrCamera {
public:
    
    /**
     *  景深的参数 & 相机的参数
     */
    struct CameraConfigure{
        double eye_to_img = 500,//眼睛（小孔）距离成像屏幕的距离
        eyeup = 100,//图像最上方距眼睛的距离
        eyedown = -300,//图像最下方距眼睛的距离
        eyeleft = -200,//图像最左方距眼睛的距离
        eyeright = 200,//图像最右方距眼睛的距离
        focus_dist = 10;//焦点距小孔（眼睛）的距离（焦距）
        //目前是x z 轴为平面的圆孔
        double aperture_range = 20;//小孔孔径（相机的光圈）
        bool open_alias = false,//是否开启抗锯齿
        open_depth = false;//是否开启景深效果
    };
    
    DrCamera(CameraConfigure& configure, DrScene& scn);
    
    /*
     *进程函数
     */
    void startProcess(cv::Mat& image);
    
    /*
     *光线追踪函数
     */
    void rayTracing(int thread_index);
    
    /*
     *抗锯齿
     */
    void antiAlias(int thread_index);
    
    /*
     *获得相机位置，建立相机坐标系（需要在startProcess之前配置完毕）
     */
    void getEyePosition(DrVector &e, DrVector &lookat, DrVector &up);
    
private:
    std::mutex * lines;
    int ** Pixels;
    int threads;
    int thread_amount[9];
    CameraConfigure conf;
    cv::Mat Result;
    DrVector u,v,w,eye;
    DrScene scene;
    
    /*
     * 针对相机坐标系，产生视线，并进行像素点到世界坐标的向量转换
     * x 为 像素第 x 列， y 为 第y行， height 为 投射屏幕与小孔的距离， nx 表示 总列数， ny 表示 总行数
     * left ， right， up， down 分别为 左右上下的坐标值 dist 为眼睛到此像素的距离
     */
    DrRay transformToGlobal(double x, double y, double height, int nx, int ny, double left, double right, double up, double down, double& dist);
    
};

#endif /* defined(__DrRayTracer__DrCamera__) */
