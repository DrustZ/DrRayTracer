//
//  DrTriangle.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/19.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrTriangle__
#define __DrRayTracer__DrTriangle__

#include "DrGeometry.h"

class DrTriangle : public DrGeometry {
public:
    DrTriangle(const DrVector &v1, const DrVector &v2, const DrVector &v3, DrPnt<DrTexture>& tex, bool ref, double refr, double trans);
    
    DrTriangle(const DrVector &v1, const DrVector &v2, const DrVector &v3);
    void getTex(DrPnt<DrTexture>& tex, bool ref, double refr, double trans);

    DrVector get_v0() const
    { return m_v0; }
    DrVector get_v1() const
    { return m_v1; }
    DrVector get_v2() const
    { return m_v2; }

    /*
     *插值
     *获得在某点法向量
     */
    DrVector getNormal(const DrVector &v);
    
    /*
     *判断是否在几何体内部
     */
    bool inside (const DrVector& vec)
    {return false;}
    
    /*
     *判断光线是否在几何体内部
     */
    bool rayInside(const DrRay& ray)
    { return false; }
    
    /*
     *获得光源到交点的距离
     */
    double intersection(const DrRay& ray);
    
    /*
     *判断是否相交
     */
    bool intersect(const DrRay& ray);
    
    /*
     *获得对应光线的折射光线，如果折射光线为全反射或者无折射则返回false
     *refraction：折射光线  point 入射点 view 视线的方向 inside 入射光线是否在几何体内
     */
    bool getRefraction(DrVector& refraction, const DrVector& point,
                       const DrVector& view, bool inside);
    
    double metrix(const DrVector &v1,const DrVector &v2,const DrVector &v3);

    int sayMyself() { return 4;}
    
    /*
     *获得顶点法向量
     */
    void setVn(DrVector &vn1, DrVector &vn2, DrVector &vn3);
    
    /*
     *获取材质
     */
//    void getAppearance(DrVector& vec, OpticalProperty& property);

    /*
     *获得顶点纹理坐标
     */
    void setText(DrVector &t1, DrVector &t2, DrVector &t3);

public:
    DrVector normal,normal_v1,normal_v2,normal_v3,
             tex0, tex1, tex2,
             m_v0, m_v1, m_v2,
             e1, e2;//v0 - v1, v0 - v2;
    double t, beta, gamma, d;
    bool insert_value, got_tex;
};

#endif /* defined(__DrRayTracer__DrTriangle__) */
