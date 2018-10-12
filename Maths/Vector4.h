//
//  Vector4.h
//  Maths
//
//  Created by Rich Davison
//  Adapted from NCLGL
#pragma once

#include <stdio.h>

class Vector4    {
public:
    Vector4(void) {
        x = y = z = w = 1.0f;
    }
    Vector4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    
    ~Vector4(void){}
    
    float x;
    float y;
    float z;
    float w;
};

