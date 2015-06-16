//
//  DrObjReader.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/26.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrObjReader.h"
#include <iostream>
#include <sstream>
#include <string>

DrObjReader::DrObjReader(char* filename):m_file(filename){}

bool DrObjReader::read(){
    using namespace std;
    ifstream f(m_file);
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)){
        if (!line.length()) continue;
        int i = 0;
        while(line[i] == ' ') ++i;
        switch (line[i]) {
            case 'v':
            {
                if (line[i+1] != 'n' && line[i+1] != 't'){
                    string x,y,z;
                    i += 1;
                    while(!valid(line[i])) ++i;
                    for(; valid(line[i]); ++i)
                        x += line[i];
                    while(!valid(line[i])) ++i;
                    for(; valid(line[i]); ++i)
                        y += line[i];
                    while(!valid(line[i])) ++i;
                    for(; valid(line[i]); ++i)
                        z += line[i];
                    double xx,yy,zz;
                    xx = stod(x);// * 100;
                    yy = stod(y);// * 100;
                    zz = stod(z);// * 100;
                    points.push_back(DrVector(xx, yy, zz));
                }
            }
                break;
                
            case 'f':
            {
                vector<string> internal;
                stringstream ss(line.substr(i+2)); // Turn the string into a stream.
                string tok;
                while(getline(ss, tok, ' ')) {
                    if (tok != " ")
                        internal.push_back(tok);
                }
                if (internal.size() == 3){
                    int index1, index2, index3;
                    index1 = atoi(internal[0].c_str());
                    index2 = atoi(internal[1].c_str());
                    index3 = atoi(internal[2].c_str());
                    if (index1 < 0){
                        index1 = int(points.size()) + index1;
                        index2 = int(points.size()) + index2;
                        index3 = int(points.size()) + index3;
                    } else {
                        index1 -= 1;
                        index2 -= 1;
                        index3 -= 1;
                    }
                    triangles.push_back(DrTriangle(points[index1], points[index2], points[index3]));
                }
                else {
                    int index1, index2, index3, index4;
                    index1 = atoi(internal[0].c_str());
                    index2 = atoi(internal[1].c_str());
                    index3 = atoi(internal[2].c_str());
                    index4 = atoi(internal[3].c_str());
                    if (index1 < 0){
                        index1 = int(points.size()) + index1;
                        index2 = int(points.size()) + index2;
                        index3 = int(points.size()) + index3;
                        index4 = int(points.size()) + index4;
                    } else {
                        index1 -= 1;
                        index2 -= 1;
                        index3 -= 1;
                        index4 -= 1;
                    }
                    triangles.push_back(DrTriangle(points[index1], points[index2], points[index3]));
                    triangles.push_back(DrTriangle(points[index3], points[index4], points[index1]));
                }
            }
                break;
            default:
                break;
        }
    }
    cout << triangles.size();
    return true;
}

void DrObjReader::load(DrScene& scene){
    for (int i = 0; i < triangles.size(); ++i) {
        OpticalProperty prop;
        prop.diffuse = WHITE;
        prop.specular = 0;//DrColor(0.3,0.3,0.3);
        prop.spec_exp = 222;
        prop.transparency = 0;
        prop.reflection = 0.0;
        
//        DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrImageTexture("/Users/zmr/Pictures/pictures/dots.png", prop, 2, 2));
        DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrUniformTexture(prop));

        triangles[i].getTex(texture, false, 0, 1.5);
        DrPnt<DrGeometry> shape = DrPnt<DrGeometry>(&triangles[i]);
        scene.addObj(shape);
    }
}

double DrObjReader::stod(std::string &s){
    using namespace std;
    double num = 0;
    int cnt = 0;
    bool neg = false, frac = false;
    for (int i = 0; i < s.length(); ++i){
        if (s[i] == '-') { neg = true; continue; }
        if (s[i] == '.') { cnt = 1; frac = true; continue; }
        if ('0' > s[i] || s[i] > '9') continue;
        if (frac) { num += (s[i] - '0') / pow(10, cnt);  cnt++; continue; }
        num = (s[i] - '0') + num * 10;
    }
    if (neg) num = -num;
    return num;
}

bool DrObjReader::valid(char &n){
    if (('0' <= n && n <= '9') || (n == '-') || (n == '.') ) return true;
    return false;
}