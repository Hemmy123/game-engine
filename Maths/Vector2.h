//
//  Vector2.h
//  Maths
//
//  Created by Rich Davison
//  Adapted from NCLGL

#pragma once


#include <stdio.h>
#include <iostream>
#include <math.h>
class Vector2{
public:
    
    Vector2(void){
        ToZero();
    }
    
    Vector2(const float x, const float y) {
        this->x = x;
        this->y = y;
    }
    
    ~Vector2(void){}
    
    float x;
    float y;
    
    inline void ToZero(){
        x = 0.0f;
        y = 0.0f;
    }
    
    
    void Invert() {
        x = -x;
        y = -y;
    }

    inline Vector2  operator-(const Vector2  &a) const{
        return Vector2(x - a.x,y - a.y);
    }
    inline Vector2  operator-() const{
        return Vector2(-x,-y);
    }
    inline Vector2  operator+(const Vector2  &a) const{
        return Vector2(x + a.x,y + a.y);
    }
    
    inline Vector2  operator*(const float &a) const {
        return Vector2(x * a, y * a );
    }
    
    
    inline float dot(const Vector2 &a) const {
        return (x * a.x + y * a.y);
    }
    
    
    
    void            Normalise() {
        float length = Length();
        
        if (length != 0.0f) {
            length = 1.0f / length;
            x = x * length;
            y = y * length;
        }
    }
    
    float            Length() const {
        return sqrt((x*x) + (y*y));
    }

    
    inline friend std::ostream& operator<<(std::ostream& o, const Vector2& v){
        o << "Vec2(" << v.x << "," << v.y << ")";
        return o;
    }
    
    
};

