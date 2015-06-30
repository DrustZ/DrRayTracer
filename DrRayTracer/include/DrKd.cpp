//
//  DrKd.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/6/29.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrKd.h"
#include <cstdlib>
#include <cstdio>
using namespace std;

std::vector< KdTriInfo > Triangles;
double min3(double a, double b, double c){
    if (a < b){
        if (a < c) return a;
        else return c;
    } else if (b < c) return b;
    return c;
}

double max3(double a, double b, double c){
    if (a > b){
        if (a > c) return a;
        else return c;
    } else if (b > c) return b;
    return c;
}

DrKd::DrKd(int amount){
    size = amount;
    type = leaf = empty = lx = ly = lz = bx = by = bz = 0;
    lson = rson = NULL;
    bestpos = -1;
    bxsorted = new int[size], lysorted = new int[size];
    bysorted = new int[size], lzsorted = new int[size];
    bzsorted = new int[size], lxsorted = new int[size];
}

void DrKd::build(int depth){
    
    for (int i = 0; i < size; ++i)
        bxsorted[i] = lysorted[i] = bysorted[i] = lzsorted[i] = bzsorted[i] = lxsorted[i];
    //start building
    int * lmeshes = NULL;
    int * bmeshes = NULL;
    sort();
    if (depth == 0){
        lx = lxsorted[0], ly = lysorted[0], lz = lzsorted[0];
        bx = bxsorted[size-1], by = bysorted[size-1], bz = bzsorted[size-1];
        lx = Triangles[lx].p[Triangles[lx].lx].x;
        bx = Triangles[bx].p[Triangles[bx].bx].x;
        ly = Triangles[ly].p[Triangles[ly].ly].y;
        by = Triangles[by].p[Triangles[by].by].y;
        lz = Triangles[lz].p[Triangles[lz].lz].z;
        bz = Triangles[bz].p[Triangles[bz].bz].z;
        printf("lx: %f, bx: %f, ly: %f, by %f, lz %f, bz %f\n", lx, bx, ly, by, lz, bz);
    }

    //edge conditions
    if (depth > MAXDEP){
        leaf = true;
        return ;
    } else if (size <= LESSTRI){
        leaf = true;
        if (size == 0)
            empty = true;
        return ;
    }
//    printf("start \n");
        if (type == 1){
        lmeshes = lxsorted;
        bmeshes = bxsorted;
    } else if (type == 2){
        lmeshes = lysorted;
        bmeshes = bysorted;
    } else {
        lmeshes = lzsorted;
        bmeshes = bzsorted;
    }
    double bestcost = 400000000;
    double cost = 0;
    int lsize = 0;
    for (int i = 0; i < size; ++i){
        double pos = calculatecost( Triangles[lmeshes[i]], cost );
        cost /= 1000.0;
//        printf("cost : %f, best: %f\n", cost, bestcost);
        if (cost < bestcost){
            bestcost = cost, bestpos = pos;
        }
    }

//find how many tris in the left and right
    int* overlap = new int[size];
    int olamt = 0;
    switch(type){
        case 1: {
            if (bestpos == bx){
                leaf = true;
                return;
            }
            lsize = find(bestpos, bxsorted, 1) + 1;
            for (int i = lsize ; i < size ; ++i)
                if (Triangles[bxsorted[i]].p[Triangles[bxsorted[i]].lx].x <= bestpos)//三角形的最大x点超过了bestpos但是最小没有，此时为交叉
                    overlap[olamt++] = bxsorted[i];
            lson = new DrKd(lsize+olamt);
            rson = new DrKd(size - lsize);
            lson->type = rson->type = 2;
            lson->lx = lx, lson->bx = bestpos, lson->ly = ly, lson->by = by, lson->lz = lz, lson->bz = bz;
            rson->lx = bestpos, rson->bx = bx, rson->ly = ly, rson->by = by, rson->lz = lz, rson->bz = bz;
            for (int i = 0; i < lsize; ++i)
                lson->lxsorted[i] = bxsorted[i];
            for (int i = 0; i < olamt; ++i)
                lson->lxsorted[lsize+i] = overlap[i];
            for (int i = lsize; i < size; ++i)
                rson->lxsorted[i - lsize] = bxsorted[i];
            break;
        }
        case 2:{
            if (bestpos == by){
                leaf = true;
                return;
            }
            lsize = find(bestpos, bysorted, 3) + 1;
            for (int i = lsize ; i < size ; ++i)
                if (Triangles[bysorted[i]].p[Triangles[bysorted[i]].ly].y <= bestpos)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
                    overlap[olamt++] = bysorted[i];
            lson = new DrKd(lsize+olamt);
            rson = new DrKd(size - lsize);
            lson->type = rson->type = 3;
            lson->lx = lx, lson->bx = bx, lson->ly = ly, lson->by = bestpos, lson->lz = lz, lson->bz = bz;
            rson->lx = lx, rson->bx = bx, rson->ly = bestpos, rson->by = by, rson->lz = lz, rson->bz = bz;            for (int i = 0; i < lsize; ++i)
                lson->lxsorted[i] = bysorted[i];
            for (int i = 0; i < olamt; ++i)
                lson->lxsorted[lsize+i] = overlap[i];
            for (int i = lsize; i < size; ++i)
                rson->lxsorted[i - lsize] = bysorted[i];
            break;
         }
        default:{
            if (bestpos == bz){
                leaf = true;
                return;
            }
            lsize = find(bestpos, bzsorted, 5) + 1;
            for (int i = lsize ; i < size ; ++i)
                if (Triangles[bzsorted[i]].p[Triangles[bzsorted[i]].lz].z <= bestpos)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
                    overlap[olamt++] = bzsorted[i];
            lson = new DrKd(lsize+olamt);
            rson = new DrKd(size - lsize);
            lson->type = rson->type = 1;
            lson->lx = lx, lson->bx = bx, lson->ly = ly, lson->by = by, lson->lz = lz, lson->bz = bestpos;
            rson->lx = lx, rson->bx = bx, rson->ly = ly, rson->by = by, rson->lz = bestpos, rson->bz = bz;
            for (int i = 0; i < lsize; ++i)
                lson->lxsorted[i] = bzsorted[i];
            for (int i = 0; i < olamt; ++i)
                lson->lxsorted[lsize+i] = overlap[i];
            for (int i = lsize; i < size; ++i)
                rson->lxsorted[i - lsize] = bzsorted[i];
            break;
        }
    }
    printf("lsize : %d, rsize: %d, dep: %d, type : %d best: %f\n", lson->size, rson->size, depth, type,bestpos);
    lson->build(depth+1);
    rson->build(depth+1);
}

double DrKd::getIntersect(const DrRay &ray, int mode){
    if (mode == 1)
        return lson->getIntersect(ray, 3);
    if (mode == 2) 
        return rson->getIntersect(ray, 3);
    if (mode == 3)//left
    {
        /*
              p4 ______________ p5
                /|          p8 A
               /_|____________/|
            p3 | |___________|_|                   
               | /p1         | /p6
               |/____________|/ 
              p2              p7
        */
        DrVector p1 = DrVector(lx,ly,lz); DrVector p2 = DrVector(bx,ly,lz); 
        DrVector p3 = DrVector(bx,ly,bz); DrVector p4 = DrVector(lx,ly,bz); 
        DrVector p5 = DrVector(lx,by,bz); DrVector p6 = DrVector(lx,by,lz);
        DrVector p7 = DrVector(bx,by,lz); DrVector p8 = DrVector(bx,by,bz);
        double l1,l2,l3, b1,b2,b3; l1 = l2 = l3 = -100000000;b1 = b2 = b3 = 100000000;
        //左右面, l1 and b1
        DrVector normal = (p1-p2).cross(p1-p4).getNormalize();
        double d = normal * p1;
        if (testIntersect(ray, normal, d)){
            l1 = getIntersection(ray, normal, d);
            normal = (p5 - p8).cross(p5 - p6).getNormalize();
            d = normal * p5;
            if (testIntersect(ray, normal, d))
                b1 = getIntersection(ray, normal, d);
            if (l1 > b1) { l1 += b1; b1 = l1 - b1; l1 -= b1; }
        }
        else if (ray.startpoint.y < p2.y || ray.startpoint.y > p7.y) return -1;

        //上下面, l2 and b2
        normal = (p3-p4).cross(p3-p5).getNormalize();
        d = normal * p3;
        if (testIntersect(ray, normal, d)){
            l2 = getIntersection(ray, normal, d);
            normal = (p2 - p1).cross(p2 - p6).getNormalize();
            d = normal * p2;
            if (testIntersect(ray, normal, d))
                b2 = getIntersection(ray, normal, d);
            if (l2 > b2) { l2 += b2; b2 = l2 - b2; l2 -= b2; }
        }
        else if (ray.startpoint.z < p1.z || ray.startpoint.z > p8.z) return -1;

        //前后面, l3 and b3
        normal = (p2-p3).cross(p2-p8).getNormalize();
        d = normal * p2;
        if (testIntersect(ray, normal, d)){
            l3 = getIntersection(ray, normal, d);
            normal = (p5 - p4).cross(p5 - p6).getNormalize();
            d = normal * p5;
            if (testIntersect(ray, normal, d))
                b3 = getIntersection(ray, normal, d);
            if (l3 > b3) { l3 += b3; b3 = l3 - b3; l3 -= b3; }
        }
        else if (ray.startpoint.x < p1.x || ray.startpoint.x > p8.x) return -1;
//        printf(" min %f, %f, %f;  max %f, %f, %f\n", l1, l2, l3, b1, b2, b3);
        double maxmin = max3(l1, l2, l3);
        double minmax = min3(b1, b2, b3);
//        printf(" min %f, %f\n", maxmin, minmax);
        if (maxmin < minmax) return maxmin;
        else return -1;
    }
    return -1;
}

bool DrKd::testIntersect(const DrRay &ray, DrVector &normal, double d){

    double t = normal * ray.direction;
    if (getSign(t) == 0)
        return false;
    return true;
}

double DrKd::getIntersection(const DrRay &ray, DrVector &normal, double d){
    double t = normal * ray.direction;
    double dist = (d - ray.startpoint.dot(normal)) / t;
    return dist;
}

float DrKd::calculatecost( KdTriInfo tri, double& cost )
{
    double minx = tri.p[tri.lx].x; double miny = tri.p[tri.ly].y; double minz = tri.p[tri.lz].z;
    double maxx = tri.p[tri.bx].x; double maxy = tri.p[tri.by].y; double maxz = tri.p[tri.bz].z;
    double leftarea, rightarea;
    int leftcount, rightcount;
    double costtrav = 1;
    double bestcost = 0;
    int bpos = 0;
    //可以更改搜索左右面片数的方法
    if (type == 1){
        leftarea = calculateArea(minx, 1);
        rightarea = calculateArea(minx, 2);
        leftcount = find(minx, bxsorted, 1) + 1;
        rightcount = size - find(minx, lxsorted, 2);
        leftcount += (size - leftcount - rightcount);
        rightcount += (size - leftcount - rightcount);
//        for (int j = leftcount; j < size; ++j)
//            if (Triangles[bxsorted[j]].p[Triangles[bxsorted[j]].lx].x <= minx)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
//                leftcount += 1;
        cost = costtrav + (leftarea * leftcount + rightarea * rightcount);
        bpos = minx;
//        printf("leftc : %d , %f, rightc : %d , %f\n", leftcount, leftarea, rightcount, rightarea);
        bestcost = costtrav + (leftarea * leftcount + rightarea * rightcount);
        leftarea = calculateArea(maxx, 1);
        rightarea = calculateArea(maxx, 2);
        leftcount = find(maxx, bxsorted, 1) + 1;
        rightcount = size - find(maxx, lxsorted, 2);
        leftcount += (size - leftcount - rightcount);
        rightcount += (size - leftcount - rightcount);
//        for (int j = leftcount; j < size; ++j)
//            if (Triangles[bxsorted[j]].p[Triangles[bxsorted[j]].lx].x <= maxx)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
//                leftcount += 1;
//
        if (bestcost < cost){
            cost = bestcost;
            bpos = maxx;
        }
    } else if (type == 2){
        leftarea = calculateArea(miny, 1);
        rightarea = calculateArea(miny, 2);
        leftcount = find(miny, bysorted, 3) + 1;
        rightcount = size - find(miny, lysorted, 4);
        leftcount += (size - leftcount - rightcount);
        rightcount += (size - leftcount - rightcount);
//        for (int j = leftcount; j < size; ++j)
//            if (Triangles[bysorted[j]].p[Triangles[bysorted[j]].ly].y <= miny)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
//                leftcount += 1;
        cost = costtrav + (leftarea * leftcount + rightarea * rightcount);
        bpos = miny;
        leftarea = calculateArea(maxy, 1);
        rightarea = calculateArea(maxy, 2);
        leftcount = find(maxy, bysorted, 3) + 1;
        rightcount = size - find(maxy, lysorted, 4);
        leftcount += (size - leftcount - rightcount);
        rightcount += (size - leftcount - rightcount);
//        for (int j = leftcount; j < size; ++j)
//            if (Triangles[bysorted[j]].p[Triangles[bysorted[j]].ly].y <= maxy)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
//                leftcount += 1;
        bestcost = costtrav + (leftarea * leftcount + rightarea * rightcount);
        if (bestcost < cost){
            cost = bestcost;
            bpos = maxy;
        }
    } else {
        leftarea = calculateArea(minz, 1);
        rightarea = calculateArea(minz, 2);
        leftcount = find(minz, bzsorted, 5) + 1;
        rightcount = size - find(minz, lzsorted, 6);
        leftcount += (size - leftcount - rightcount);
        rightcount += (size - leftcount - rightcount);
//        for (int j = leftcount; j < size; ++j)
//            if (Triangles[bysorted[j]].p[Triangles[bzsorted[j]].lz].z <= minz)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
//                leftcount += 1;
        cost = costtrav + (leftarea * leftcount + rightarea * rightcount);
        bpos = minz;
        leftarea = calculateArea(maxz, 1);
        rightarea = calculateArea(maxz, 2);
        leftcount = find(maxz, bzsorted, 5) + 1;
        rightcount = size - find(maxz, lzsorted, 6);
        leftcount += (size - leftcount - rightcount);
        rightcount += (size - leftcount - rightcount);
//        for (int j = leftcount; j < size; ++j)
//            if (Triangles[bysorted[j]].p[Triangles[bzsorted[j]].lz].z <= maxz)//三角形的最大y点超过了bestpos但是最小没有，此时为交叉
//                leftcount += 1;
        bestcost = costtrav + (leftarea * leftcount + rightarea * rightcount);
        if (bestcost < cost){
            cost = bestcost;
            bpos = maxz;
        }
    }
    return bpos;
} 

double DrKd::calculateArea(double split, int mode/*1 for left, 2 for right*/){
    double yl = by - ly;
    double xl = bx - lx;
    double zl = bz - lz;
    if (mode == 1){
        if (type == 1) xl = split - lx;
        else if (type == 2) yl = split - ly;
        else zl = split - lz;
    } else {
        if (type == 1) xl = bx - split;
        else if (type == 2) yl = by - split;
        else zl = bz - split;
    }
    return 2 * yl * zl  // y * z face area
         + 2 * yl * xl  // x * y face area
         + 2 * zl * xl; // z * x face area
}

int DrKd::find(double target, int * group, int mode){
    //mode : 1 maxx 2 minx 3 maxy 4 miny 5 maxz 6 minz
    int low = 0, high = size;
    while(low < high){
        int middle = (low + high) >> 1;
        double compare = 0;
        switch(mode){
            case 1: compare = Triangles[group[middle]].p[Triangles[group[middle]].bx].x; break;
            case 2: compare = Triangles[group[middle]].p[Triangles[group[middle]].lx].x; break;
            case 3: compare = Triangles[group[middle]].p[Triangles[group[middle]].by].y; break;
            case 4: compare = Triangles[group[middle]].p[Triangles[group[middle]].ly].y; break;
            case 5: compare = Triangles[group[middle]].p[Triangles[group[middle]].bz].z; break;
            default: compare = Triangles[group[middle]].p[Triangles[group[middle]].lz].z; break;
        }
        (target < compare) ? high = middle : low = middle + 1;
    }
    return low - 1;
}

bool compbx(const int& p1, const int& p2){
    return Triangles[p1].p[Triangles[p1].bx].x < Triangles[p2].p[Triangles[p2].bx].x;
}
bool compby(const int& p1, const int& p2){
    return Triangles[p1].p[Triangles[p1].by].y < Triangles[p2].p[Triangles[p2].by].y;
}
bool compbz(const int& p1, const int& p2){
    return Triangles[p1].p[Triangles[p1].bz].z < Triangles[p2].p[Triangles[p2].bz].z;
}
bool complx(const int& p1, const int& p2){
    return Triangles[p1].p[Triangles[p1].lx].x < Triangles[p2].p[Triangles[p2].lx].x;
}
bool comply(const int& p1, const int& p2){
    return Triangles[p1].p[Triangles[p1].ly].y < Triangles[p2].p[Triangles[p2].ly].y;
}
bool complz(const int& p1, const int& p2){
    return Triangles[p1].p[Triangles[p1].lz].z < Triangles[p2].p[Triangles[p2].lz].z;
}

void DrKd::sort(){
    std::sort(lxsorted,lxsorted+size,complx);
    std::sort(bxsorted,bxsorted+size,compbx);
    std::sort(lysorted,lysorted+size,comply);
    std::sort(bysorted,bysorted+size,compby);
    std::sort(lzsorted,lzsorted+size,complz);
    std::sort(bzsorted,bzsorted+size,compbz);
}
