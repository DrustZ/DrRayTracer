//
//  DrObjReader.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/26.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrObjReader__
#define __DrRayTracer__DrObjReader__

#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include "DrMath.h"
#include "DrScene.h"
#include "DrTriangle.h"
#include "DrImageTexture.h"
#include "DrUniformTexture.h"

class DrObjReader{
public:
    DrObjReader(char * filename, double r);
    bool read();
    void load(DrScene& scene);
    double stod(const std::string& s);
    bool valid(char &n);
    std::string trim(const std::string& s);

private:
    char * m_file;
    double rate = 2;
    std::vector<DrVector> points;
    std::vector<DrVector> tex_ord;
    std::vector<DrVector> pnt_normal;
    std::vector<DrTriangle> triangles;
};

#endif /* defined(__DrRayTracer__DrObjReader__) */
