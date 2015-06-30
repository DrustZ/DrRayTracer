//
//  DrMath.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrMath__
#define __DrRayTracer__DrMath__
#include "DrColor.h"
#include <cmath>
#include <cassert>
#include <iostream>

/**
 * 比较函数 大于0 返回1 小于0 返回-1
 */
static inline int getSign(double t){
    if (fabs(t) < DrEPS){
        return 0;
    }
    return t > 0 ? 1 : -1;
}

/**
 *  平方
 */
static inline double sqr(double t){
    return t * t;
}

/**
 *  除法 (防止 除数为 0）
 */
static inline double div(double a, double b){
    if (a == b)
        return 1;
    if (!getSign(b))
        return 1e10 * getSign(a);
    else return a/b;
}


static inline int findmax(double a, double b, double c){
    if (getSign(a - b) > 0){
        if (getSign(a - c) > 0)
            return 0;
        else return 2;
    }
    else if (getSign(b - c) > 0)
        return 1;
    return 2;
}

static inline int findmin(double a, double b, double c){
    if (getSign(a - b) < 0){
        if (getSign(a - c) < 0)
            return 0;
        else return 2;
    }
    else if (getSign(b - c) < 0)
        return 1;
    return 2;
}

//
///**
// *  root of linear equation c1 * x + c0 = 0
// *  计算成功 返回true
// */
//static bool inline calcLinearRoot(double & result, double c1, double c0){
//    if (!getSign(c1))
//        return false;
//    else {
//        result = div(-c0, c1);
//        return true;
//    }
//}
//
///**
// *  计算 二次方程 解
// *  计算成功 返回true
// */
//static bool CalcQuadRoot(double * results, double c2, double c1, double c0){
//    double d;
//    
//    if (!getSign(d)){
//        calcLinearRoot(d, c1, c0);
//        results[0] = results[1] = d;
//        return true;
//    }
//    else
//    {
//        d = c1 * c1 - 4 * c2 * c0;
//        
//        if (!getSign(d))
//        {
//            results[0] = div(-c1, 2 * c2);
//        }
//        else
//        {
//            if (d > 0)
//            {
//                results[0] = div(-c1 + sqrt(d), 2 * c2);
//                results[1] = div(-c1 - sqrt(d), 2 * c2);
//            }
//            else
//                return false;
//        }
//    }
//    return true;
//}

/**
 *  三维向量
 */
struct DrVector
{
    double x,y,z;
    DrVector(double xx = 0, double yy = 0, double zz = 0): x(xx),y(yy),z(zz){}
    DrVector(const DrVector &v): x(v.x), y(v.y), z(v.z){}
    /**
     *  从v1 到 v2 的向量
     */
    DrVector(const DrVector &v1, const DrVector &v2):
    x(v2.x - v1.x), y(v2.y - v1.y), z(v2.z - v1.z){}
    
    /**
     *  获取三者最小绝对值
     */
    double getAbsMin() const{
        double a = fabs(x);
        double b = fabs(y);
        double c = fabs(z);
        double min = 0;
        if (a < b)
            min = a;
        else min = b;
        if (min < c) min = c;
        return min;
    }
    
    /**
     *  取模
     */
    double modulus() const{
        return sqrt(sqr(x) + sqr(y) + sqr(z));
    }
    
    /**
     *  平方
     */
    double sqrModulus() const{
        return sqr(x) + sqr(y) + sqr(z);
    }
    /**
     *  对向量本身单位化
     */
    void normalize(){
        double m = modulus();
        x = div(x, m);
        y = div(y, m);
        z = div(z, m);
    }
    
    /**
     *  获得单位化向量
     */
    DrVector getNormalize(){
        double m = modulus();
        return DrVector(div(x, m), div(y, m), div(z, m));
    }
    
    /**
     *  符号重载
     */
    DrVector operator + (DrVector const &v) const{
        return DrVector(x + v.x, y + v.y, z + v.z);
    }
    
    DrVector& operator += (DrVector const &v){
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    
    DrVector operator - (DrVector const &v) const{
        return DrVector(x - v.x, y - v.y, z - v.z);
    }
    
    DrVector operator - () const{
        return DrVector(-x, -y, -z);
    }
    
    DrVector& operator -= (DrVector const &v){
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    
    DrVector operator * (double f){
        return DrVector(x * f, y * f, z * f);
    }
    
    double operator * (DrVector const &v){
        return x * v.x + y * v.y + z * v.z;
    }
    
    DrVector& operator *= (double f){
        x *= f; y *= f; z *= f;
        return *this;
    }
    
    friend std::ostream& 
    operator << (std::ostream& os, const DrVector& d) {
        return os << " (" << d.x << "," << d.y << "," << d.z << ") ";
    }
    /**
     *  点积
     */
    double dot(DrVector const &v) const{
        return x * v.x + y * v.y + z * v.z;
    }
    
    /**
     *  叉积
     */
    DrVector cross(DrVector const &v) const{
        return DrVector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    /**
     *  判断是否为零向量
     */
    bool empty() const{
        return (!getSign(x)) && (!getSign(y)) && (!getSign(z));
    }
    
    /**
     *  以v为法向量，获得以本向量负向量为入射向量时的反射向量
     *  注意：两向量必须为单位向量
     */
    DrVector reflection(DrVector const &v) const{
        assert(!getSign(this->modulus() - 1) && !getSign(v.modulus() - 1));
        return DrVector(DrVector(v.x * dot(v)*2, v.y * dot(v)*2, v.z * dot(v)*2) - *this);
    }
};

struct KdTriInfo{
    int idx;
    DrVector p[3];
    int bx, lx, by, ly, bz, lz;
    void getIdx(int i){idx = i;}
    void getPnts(const DrVector& p11,const DrVector& p22,const DrVector& p33){
        p[0] = p11; p[1] = p22; p[2] = p33;
        bx = findmax(p11.x, p22.x, p33.x);
        by = findmax(p11.y, p22.y, p33.y);
        bz = findmax(p11.z, p22.z, p33.z);
        lx = findmin(p11.x, p22.x, p33.x);
        ly = findmin(p11.y, p22.y, p33.y);
        lz = findmin(p11.z, p22.z, p33.z);
    }
};

#endif /* defined(__DrRayTracer__DrMath__) */
