
////========================================
// Class: Utils.h
// Author: Hemmy
// Date: 02/06/2018.
// Description:
// Some basic math functions, adapted from NCLGL
//
// ========================================
#pragma once


//#include <math.h> 
#include <cstdlib>
#include <limits>
#include <stdio.h>

static const float  PI = 3.14159265358979323846f;


class MathUtils {
    
public:
    //Degrees to radians
     static inline float DegToRad(const float rad)    {
        return rad * PI / 180.0f;
    };

	 static inline float RadToDeg(const float rad) {
		 return rad * (180.0f / PI);
	 };


    
	static inline float max(float a, float b) { return  a > b ?  a :  b;};
	static inline float min(float a, float b) { return  a < b ?  a :  b;};
	
	
	static inline float lerp(float a, float b, float t){return a + t * (b - a);}
	
};
