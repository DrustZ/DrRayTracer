//
//  DrColor.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrColor__
#define __DrRayTracer__DrColor__

#define PI 3.14159265358979
#define DrEPS 1e-6
#include <iostream>
static inline bool range(float n){
    return (n >= 0) && (n <= 1);
}

/**
 *  颜色类
 *  注意：颜色中rgb 需要在[0 1]之间
 */
struct DrColor{
    float r,g,b;
    DrColor(float rr = 0, float gg = 0, float bb = 0): r(rr),g(gg),b(bb){}
    DrColor(const DrColor &c): r(c.r), g(c.g), b(c.b){}
    /**
     *  运算符重载
     */
    DrColor operator + (DrColor const &c) const{
        return DrColor(r + c.r, g + c.g, b + c.b);
    }
    
    DrColor& operator += (DrColor const &c){
        r += c.r; g += c.g; b += c.b;
        return *this;
    }
    
    DrColor operator - (DrColor const &c) const{
        return DrColor(r - c.r, g - c.g, b - c.b);
    }
    
    DrColor operator - () const{
        return DrColor(-r, -g, -b);
    }
    
    DrColor& operator -= (DrColor const &c){
        r -= c.r; g -= c.g; b -= c.b;
        return *this;
    }
    
    DrColor operator * (double f){
        return DrColor(r * f, g * f, b * f);
    }
    
    DrColor operator * (const DrColor &c){
        return DrColor(r * c.r, g * c.r, b * c.r);
    }
    
    DrColor& operator *= (double f){
        r *= f; g *= f; b *= f;
        return *this;
    }
    
    friend std::ostream&
    operator << (std::ostream& os, const DrColor& d) {
        return os << " (" << d.r << "," << d.g << "," << d.b << ") ";
    }
    
    /**
     *  将颜色映射到[0 1]范围内
     */
    void setToRange(){
        if (checkRange()) return;
        float small;
        if (r < g) small = r;
        else small = g;
        if (small > b) small = b;
        
        if (small < 0){
            r -= small; g -= small; b -= small;
        }
        
        float big;
        if (r > g) big = r;
        else big = g;
        if (big < b) big = b;
        if (big > 1){
            r /= big; g /= big; b /= big;
        }
    }
    
    /**
     *  检查是否在法定范围
     */
    bool checkRange(){
        return ::range(r) && ::range(g) && ::range(b);
    }
    
};

static DrColor BLACK, WHITE = DrColor(1,1,1);
#endif /* defined(__DrRayTracer__DrColor__) */
