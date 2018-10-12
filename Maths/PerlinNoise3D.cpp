////========================================
// Class: PerlinNoise3D
// Author: Hemmy
// Date: 09/08/2018
// Description:
//
// ========================================

#include "PerlinNoise3D.h"
#include <stdio.h>
#include <numeric>
#include <vector>
#include <random>
#include <algorithm>
#include "MathUtils.h"

PerlinNoise3D::PerlinNoise3D(int destScale, int seed) :permutationSize(destScale), destScale(destScale)
{
	m_surroundingPoints = new Vector3[8];
	m_weights = new float[8];
	generatePermArray(seed,destScale);
}

PerlinNoise3D::~PerlinNoise3D(){
	
	delete[] m_weights;
	delete[] m_surroundingPoints;
}


void PerlinNoise3D::generatePermArray(int seed, int destScale){
	
	// Random Permutation taken from:
	//https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
	
	m_perm.resize(destScale);
	// Fill p with values from 0 to 255
	iota(m_perm.begin(), m_perm.end(), 0);
	
	// Initialize a random engine with seed
	std::default_random_engine engine(seed);
	
	// Suffle using the above random engine
	shuffle(m_perm.begin(), m_perm.end(), engine);
	
	// Duplicate the permutation vector
	m_perm.insert(m_perm.end(), m_perm.begin(), m_perm.end());
}


float PerlinNoise3D::fadeFunction(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}


Vector3* PerlinNoise3D::surroundingPointsOf(Vector3 point){
	
	
	Vector3 p0 = Vector3(floor(point.x), floor(point.y), floor(point.z));
	Vector3 p1 = p0 + Vector3(1,0,0);
	Vector3 p2 = p0 + Vector3(0,1,0);
	Vector3 p3 = p0 + Vector3(1,1,0);
	Vector3 p4 = p0 + Vector3(0,0,1);
	Vector3 p5 = p0 + Vector3(1,0,1);
	Vector3 p6 = p0 + Vector3(0,1,1);
	Vector3 p7 = p0 + Vector3(1,1,1);

	m_surroundingPoints[0] = p0;
	m_surroundingPoints[1] = p1;
	m_surroundingPoints[2] = p2;
	m_surroundingPoints[3] = p3;
	m_surroundingPoints[4] = p4;
	m_surroundingPoints[5] = p5;
	m_surroundingPoints[6] = p6;
	m_surroundingPoints[7] = p7;

	
	return m_surroundingPoints;
	
}



float* PerlinNoise3D::surroundingWeights(Vector3 point){
	
	Vector3* m_surroundingPoints = surroundingPointsOf(point);
	
	for (unsigned i = 0; i < 8; ++i) {
		int x = (m_surroundingPoints[i].x);
		int y = (m_surroundingPoints[i].y);
		int z = (m_surroundingPoints[i].z);
		
		int px = abs(x %permutationSize);
		int py = abs(y %permutationSize);
		int pz = abs(z %permutationSize);

		
		// using all x y z coords to get get unique perm number
		int perm1 = m_perm[py];
		int perm2 = m_perm[px + perm1];
		int perm3 = m_perm[pz + perm2];
		int gradIndex = perm3 % 11;
		
		
		Vector3 gradientVector = randomVectors[gradIndex];
		Vector3 cornerToPoint = m_surroundingPoints[i] - point ;
		
		float dotProduct = Vector3::Dot(cornerToPoint,gradientVector);
		m_weights[i] = dotProduct;
	}
	
	
	return m_weights;
}



float PerlinNoise3D::noiseAt(Vector3 point, int octaves, float frequency, float persistence){
	float freq = frequency;
	float amplitude = 1.0f;
	float total = 0.0f;
	
	for (int i = 0; i < octaves; ++i) {
		float s = (float)destScale / (float)freq;
		
		Vector3 pos = Vector3(point.x / s, point.y / s, point.z / s);
		total += noiseAt(pos) * amplitude;
		
		amplitude *= persistence;
		freq *= 2;
		
	}
	return total;
}

float PerlinNoise3D::noiseAt(Vector3 point){
	
	//Integer part of float
	int intX = floor(point.x);
	int intY = floor(point.y);
	int intZ = floor(point.z);
	
	// Decimal part of float
	float relativeX = point.x - intX;
	float relativeY = point.y - intY;
	float relativeZ = point.z - intZ;

	
	float* weights = surroundingWeights(point);
	
	
	// With fading
	float fadeX = fadeFunction(relativeX);
	float fadeY = fadeFunction(relativeY);
	float fadeZ = fadeFunction(relativeZ);

	
	
	/*
	
	 bottom horizontal
	 bot1 = 0,1
	 bot2 = 4,5
	 
	 bot = lerp(bot1, bot2)
	 
	 top horizontal
	 top1 = 2,3
	 top2 = 6,7
	 top = lerp(top1, top2)
	 
	 final = lerp (bot,top);
	 
	*/
	
	float bot1 = MathUtils::lerp(m_weights[0], m_weights[1], fadeX);
	float bot2 = MathUtils::lerp(m_weights[4], m_weights[5], fadeX);
	float bot = MathUtils::lerp(bot1, bot2, fadeZ);

	float top1 = MathUtils::lerp(m_weights[2], m_weights[3], fadeX);
	float top2 = MathUtils::lerp(m_weights[6], m_weights[7], fadeX);
	float top = MathUtils::lerp(top1, top2, fadeZ);
	
	float finalLerp = MathUtils::lerp(bot,top,fadeY);

	
//	std::cout << "noise: " << finalLerp << std::endl;
//	std::cout << "point: " << point << std::endl;

	
	return finalLerp;
	

	
}










