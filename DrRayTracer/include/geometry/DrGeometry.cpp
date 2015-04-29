//
//  DrGeometry.cpp
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#include "DrGeometry.h"

DrGeometry::DrGeometry(DrPnt<DrTexture> tex, bool ref, double trans) : m_texture(tex), reflective(ref), transparency(trans), absorb(BLACK){
    
}



