////========================================
// Class: PerlinNoise
// Author: Hemmy
// Date: 28/07/2018
// Description:
//
// ========================================

#pragma once

#include <stdio.h>
#include <numeric>
#include <vector>
#include <random>
#include <algorithm>

#include "Vector3.h"
#include "Vector2.h"

using std::vector;

class PerlinNoise2D
{
public:
	
	PerlinNoise2D(int destScale, int seed);
	~PerlinNoise2D();
	
	float noiseAt(Vector2 point);
	float noiseAt(Vector2 point, int octaves, float frequency, float persistence);
	

	float	fadeFunction(float t);
	void	generatePermArray(int seed, int destScale);
	float*	surroundingWeights(Vector2 point);
	Vector2* surroundingPointsOf(Vector2 point);
	
	void setDestScale(int s) { destScale = s; }
	
protected:
	

	
	int permutationSize;
	
	vector<int> perm; // Permuation Array
	
	vector<Vector2> randomVectors = {
		Vector2(1,1),	Vector2(-1,1),
		Vector2(1,-1),	Vector2(-1,-1),
		Vector2(0,1),	Vector2(1,0),
		Vector2(1,-1),	Vector2(-1,0)
	};
	
	
	int destScale;
	
	
};

