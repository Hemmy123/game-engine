////========================================
// Class: HeightMap
// Author: Hemmy
// Date: 28/07/2018
// Description:
//
// ========================================

#pragma once


#include <stdio.h>
#include "Mesh.h"
#include "PerlinNoise2D.h"
#include "PerlinNoise3D.h"
class HeightMap:public Mesh{
public:
	
	HeightMap(
	int rawWidth,
	int rawHeight,
	float heightMap_x,
	float heightMap_z,
	float heightMap_y,
	float heightMap_tex_x,
	float heightMap_tex_z,
	PerlinNoise2D* perlinNoise);
	
	~HeightMap();
	
	void generateFlatTerrain();
	void generateRandomTerrain(Vector3 position,int octaves, float frequency, float persistance, float min, float max );
	void updateTerrain(PerlinNoise3D* perlin3D, Vector3 position,int octaves, float frequency, float persistance);
	

	int getRawWidth() const { return m_rawWidth; };
	int getRawHeight() const { return m_rawHeight; };

	float getXMilti() const { return m_xMultiplier; };
	float getZMilti() const { return m_zMultiplier; };
	float getYMilti() const { return m_yMultiplier; };

	float getXTex() const { return m_xTexCoord; };
	float getZTex() const { return m_zTexCoord; };

	Vector3 getPosition() const { return m_position; };


private:

	Vector3 m_position;
	int m_rawWidth;
	int m_rawHeight;
	
	float m_xMultiplier;
	float m_zMultiplier;
	float m_yMultiplier;
	
	float m_xTexCoord;
	float m_zTexCoord;
	PerlinNoise2D* m_noiseGenerator;
	
};


 /* HeightMap_hpp */
