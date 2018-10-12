////========================================
// Class: PerlinNoise
// Author: Hemmy
// Date: 28/07/2018
// Description:
//
// ========================================

#include "PerlinNoise2D.h"
#include "MathUtils.h"

PerlinNoise2D::PerlinNoise2D(int destScale, int seed) :permutationSize(destScale), destScale(destScale)
{
	generatePermArray(seed,destScale);
}

PerlinNoise2D::~PerlinNoise2D(){
}


void PerlinNoise2D::generatePermArray(int seed, int destScale)
{
	
	// Random Permutation taken from:
	//https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
	
	perm.resize(destScale);
	// Fill p with values from 0 to 255
	iota(perm.begin(), perm.end(), 0);
	
	// Initialize a random engine with seed
	std::default_random_engine engine(seed);
	
	// Suffle using the above random engine
	shuffle(perm.begin(), perm.end(), engine);
	
	// Duplicate the permutation vector
	perm.insert(perm.end(), perm.begin(), perm.end());
	
}

Vector2* PerlinNoise2D::surroundingPointsOf(Vector2 point)
{
	
	Vector2* surroundingPoints = new Vector2[4];
	
	Vector2 botLeft  = Vector2(floor(point.x), floor(point.y));
	Vector2 botRight = botLeft + Vector2(1,0);
	Vector2 topLeft  = botLeft + Vector2(0,1);
	Vector2 topRight = botLeft + Vector2(1,1);

	
	surroundingPoints[0] = topLeft;
	surroundingPoints[1] = topRight;
	surroundingPoints[2] = botLeft;
	surroundingPoints[3] = botRight;
	
	return surroundingPoints;
}



float PerlinNoise2D::noiseAt(Vector2 point)
{
	
	// Decimal part of float
	float relativeX = point.x - floor(point.x);
	float relativeY = point.y - floor(point.y);
	
	float* weights = surroundingWeights(point);
	
	
	// With fading
	float fadeX = fadeFunction(relativeX);
	float fadeY = fadeFunction(relativeY);
	
	float lerpA = MathUtils::lerp(weights[0], weights[1], fadeX);
	float lerpB = MathUtils::lerp(weights[2], weights[3], fadeX);
	float lerpC = MathUtils::lerp(lerpB, lerpA, fadeY);
	
	delete[] weights;
	
	return lerpC;
}

float PerlinNoise2D::noiseAt(Vector2 point, int octaves, float frequency, float persistence)
{
	
	float freq = frequency;
	float amplitude = 1.0f;
	float total = 0.0f;
	
	for (int i = 0; i < octaves; ++i) {
		float s = (float)destScale / (float)freq;
		
		Vector2 pos = Vector2(point.x / s, point.y / s);
		total += noiseAt(pos) * amplitude;
		
		amplitude *= persistence;
		freq *= 2;
		
	}
	return total;
}


float* PerlinNoise2D::surroundingWeights(Vector2 point) {
	Vector2* surroundingPoints = surroundingPointsOf(point);
	
	float* weights = new float[4];
	
	for (unsigned i = 0; i < 4; ++i) {
		int x = (surroundingPoints[i].x);
		int y = (surroundingPoints[i].y);
		
		int px = abs(x %permutationSize);
		int py = abs(y %permutationSize);
		
		
		
		int gradIndex = perm[px + perm[py ] ] % 7;
		
		Vector2 gradientVector = randomVectors[gradIndex];
		Vector2 cornerToPoint = surroundingPoints[i] - point ;
		
		float dotProduct = cornerToPoint.dot(gradientVector);
		weights[i] = dotProduct;
	}
	delete[] surroundingPoints;
	return weights;
	
}

float PerlinNoise2D::fadeFunction(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}
