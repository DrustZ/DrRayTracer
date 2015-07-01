//
//  DrObjReader.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/5/26.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrObjReader.h"
#include "DrUniformTexture.h"
#include <iostream>
#include <sstream>
#include <string>
DrObjReader::DrObjReader(char* filename, double r):m_file(filename),rate(r){}

bool DrObjReader::read(){
    using namespace std;
    //texture prepare
    bool open_mtl = false, has_mtl = false;
    string mtl_name;
    double ns, ni, d, tr, tf, ks;
    ns = ni = tr = tf = ks = 0;
    d = 1;
    stringstream ss;
    double kd[] = {0,0,0};
    OpticalProperty prop;
    prop.diffuse = WHITE;
    prop.specular = 0;//DrColor(0.3,0.3,0.3);
    prop.spec_exp = 222;
    prop.transparency = 0;
    prop.reflection = 0.0;
    DrPnt<DrTexture> texture = DrPnt<DrTexture>(new DrUniformTexture(prop));

    //read obj
    ifstream f(m_file);
    ifstream mtl;
    if (!f.is_open()) return false;
    string line;
    while (getline(f, line)){
        if (!line.length()) continue;
        int i = 0;
        while(line[i] == ' ') ++i;
        int idx = int(line.find("mtllib"));
        if (idx != string::npos) {
            string s = trim(line.substr(idx+6)) + 'h';
            string addr = "/Users/zmr/codes/c_c++/Graphics/DrRayTracer/DrRayTracer/OBJS/"+trim(line.substr(idx+6));
            cout << addr;
            mtl.open(addr);
            if (mtl.is_open()){
                open_mtl = true; cout << "opened mtl!"<<endl;
            }
        }
        idx = int(line.find("usemtl"));
        //read mtl
        if (idx != string::npos && open_mtl) {
            has_mtl = true;
            mtl_name = trim(line.substr(idx+7));

            string ll;
            bool begin = false;
            while (getline(mtl, ll)){
                if (trim(ll).empty() || ll[0] == '#'){
                    if (begin) break;
                    else continue;  }
                int tmpidx = 0;
                if (begin){
                    if (( tmpidx = int(ll.find("Ns")) ) != string::npos) ns = stod(ll.substr(tmpidx+2));
                    else if (( tmpidx = int(ll.find("Tr")) ) != string::npos) tr = stod(ll.substr(tmpidx+2));
                    else if (( tmpidx = int(ll.find("Kd")) ) != string::npos) {
                        cout << ll << endl;
                        string tmp = ll.substr(tmpidx+2);
                        ss.clear();
                        ss.str(tmp);
                        ss >> kd[0] >> kd[1] >> kd[2];
                    }
                    else if (( tmpidx = int(ll.find("Ks")) ) != string::npos) ks = stod(ll.substr(tmpidx+2));
                    else if (( tmpidx = int(ll.find("d")) ) != string::npos) d = stod(ll.substr(tmpidx+1));
                    continue;
                }
                tmpidx = int(ll.find("newmtl"));
                if (tmpidx != string::npos && ll.find(mtl_name) != string::npos){
                    begin = true;
                }
            }

            if (has_mtl){
                prop.diffuse = DrColor(kd[0],kd[1],kd[2]);
                cout << prop.diffuse << endl;
                prop.spec_exp = (ns == 0) ? 1000 : ns;
                prop.specular = 0.1;
                prop.reflection = ks;
                prop.transparency = (tr == 0) ? ( (d == 1) ? 0 : 1 - d) : tr;
                texture = DrPnt<DrTexture>(new DrUniformTexture(prop));
            }
        } else {
            //read obj
            switch (line[i]) {
                case 'v':
                {
                    //read normals
                    if (line[i+1] == 'n'){
                        string x,y,z;
                        i += 2;
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
                        xx = stod(x) * rate;
                        yy = stod(y) * rate;
                        zz = stod(z) * rate;
                        pnt_normal.push_back(DrVector(xx, yy, zz).getNormalize());
//                        printf("norm : %f, %f, %f\n", xx, yy, zz);
                    }
                    //read texture ordinates
                    else if (line[i+1] == 't'){
                        string x,y,z;
                        i += 2;
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
                        xx = stod(x);
                        yy = stod(y);
                        zz = stod(z);
                        tex_ord.push_back(DrVector(xx, yy, zz));
                    }
                    //read points
                    else {
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
                        xx = stod(x)* rate;
                        yy = stod(y)* rate;
                        zz = stod(z)* rate;
                        points.push_back(DrVector(xx, yy, zz));
//                        std::cout <<"v: " << xx << " " << yy  << " " << zz << std::endl;
                    }
                }
                    break;
                    
                case 'f':
                {
                    vector<string> internal;
                    stringstream ss(line.substr(i+2)); // Turn the string into a stream.
                    string tok;
                    while(getline(ss, tok, ' ')) {
                        if (!tok.empty() && (tok[0] - '-' >= 0) )
                            internal.push_back(tok);
                    }
                    if (internal.size() == 3){
                        int index1, index2, index3;
                        int nidx1, nidx2, nidx3;
                        int tidx1, tidx2, tidx3;
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
                        bool refreaction = prop.reflection > 0 ? true : false;
                        DrTriangle tri = DrTriangle(this->points[index1], this->points[index2], this->points[index3], texture, refreaction, 0, 1.3);
                        
                        int pos = int(internal[0].find('/'));
                        int pos2 = int(internal[1].find('/'));
                        int pos3 = int(internal[2].find('/'));
                        if (pos != string::npos){
                            if (internal[0][pos+1] != '/' && internal[0][pos+1] != ' '){
                                tidx1 = atoi(internal[0].substr(pos+1).c_str());
                                tidx2 = atoi(internal[1].substr(pos2+1).c_str());
                                tidx3 = atoi(internal[2].substr(pos3+1).c_str());
                                internal[0] = internal[0].substr(pos+1);
                                internal[1] = internal[1].substr(pos2+1);
                                internal[2] = internal[2].substr(pos3+1);
                                tri.setText(tex_ord[tidx1-1], tex_ord[tidx2-1], tex_ord[tidx3-1]);
//                                printf("text : %d %d %d\n", tidx1, tidx2, tidx3);
                            } else {
                                internal[0] = internal[0].substr(pos+1);
                                internal[1] = internal[1].substr(pos2+1);
                                internal[2] = internal[2].substr(pos3+1);
                            }
                            pos = int(internal[0].find('/'));
                            pos2 = int(internal[1].find('/'));
                            pos3 = int(internal[2].find('/'));
                            if (pos != string::npos){
                                nidx1 = atoi(internal[0].substr(pos+1).c_str());
                                nidx2 = atoi(internal[1].substr(pos2+1).c_str());
                                nidx3 = atoi(internal[2].substr(pos3+1).c_str());
                                tri.setVn(pnt_normal[nidx1-1], pnt_normal[nidx2-1], pnt_normal[nidx3-1]);
//                                printf("n : %d %d %d\n", nidx1, nidx2, nidx3);
                            }
                        }
                        triangles.push_back(tri);
//                        cout << tri.get_v0() << tri.get_v1() << tri.get_v2() << endl;
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
                        bool refreaction = prop.reflection > 0 ? true : false;
                        
                        DrTriangle tri1 = DrTriangle(points[index1], points[index2], points[index3], texture, refreaction, 0, 1.5);
                        DrTriangle tri2 = DrTriangle(points[index3], points[index4], points[index1], texture, refreaction, 0, 1.5);
                        triangles.push_back(tri1);
                        triangles.push_back(tri2);
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
    cout << triangles.size() ;
    return true;
}

void DrObjReader::load(DrScene& scene){
    using namespace std;
    for (int i = 0; i < triangles.size(); ++i) {
        DrPnt<DrGeometry> shape = DrPnt<DrGeometry>(&triangles[i]);
       
        scene.addObj(shape);
    }
    std::cout << "gong : " << scene.getObjAmount();
}

double DrObjReader::stod(const std::string &s){
    std::stringstream ss;
    ss.clear();
    ss.str(s);
    double b ;ss >> b;
    return b;
}

bool DrObjReader::valid(char &n){
    if (('0' <= n && n <= '9') || (n == '-') || (n == '.') ) return true;
    return false;
}

std::string DrObjReader::trim(const std::string &s){
    if (s.empty())
    {
        return s;
    }
    std::string t = s;
    t.erase(0,t.find_first_not_of(' '));
    t.erase(t.find_last_not_of("\n")+1);
    t.erase(t.find_last_not_of("\r")+1);
    t.erase(t.find_last_not_of(' ') + 1);
    return t;
}