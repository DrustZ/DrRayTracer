//
//  DrRectLighter.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/29.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrRectLighter__
#define __DrRayTracer__DrRectLighter__

#include "DrLighter.h"

/*
 *长方形光源类
 */
class DrRectLighter : public DrLighter {
    DrVector m_posright, m_posdown;
    DrVector * points;
    DrVector normal;
    double d;
    
public:
    /*
     * 参数：pos : 左上角光源的位置， posright : 右上角光源位置，  posdown : 左下角光源位置
     */
    DrRectLighter(DrVector &pos, DrColor &col, double inten, DrVector &posright, DrVector &posdown);
    
    ~DrRectLighter();
    /*
     *得到矩形其余两个点
     */
    DrVector* getPoints(){
        return points;
    }
    
    double inside(const DrRay &v);
        
    int lightertype()
    { return 1; }
};

#endif /* defined(__DrRayTracer__DrRectLighter__) */

