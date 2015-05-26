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
#include "DrMath.h"
#include "DrScene.h"
#include "DrTriangle.h"
#include "DrImageTexture.h"
#include "DrUniformTexture.h"

class DrObjReader{
public:
    DrObjReader(char * filename);
    bool read();
    void load(DrScene& scene);
    
private:
    char * m_file;
    std::vector<DrVector> points;
    std::vector<DrTriangle> triangles;
};

#endif /* defined(__DrRayTracer__DrObjReader__) */
