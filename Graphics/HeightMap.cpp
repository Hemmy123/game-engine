////========================================
// Class: HeightMap
// Author: Hemmy
// Date: 28/07/2018
// Description:
//
// ========================================

#include "HeightMap.h"
#include "FilePaths.h"

HeightMap::HeightMap(
					 int rawWidth,
					 int rawHeight,
					 float heightMap_x,
					 float heightMap_z,
					 float heightMap_y,
					 float heightMap_tex_x,
					 float  heightMap_tex_z,
					 PerlinNoise2D* perlinNoise):
m_rawWidth(rawWidth),
m_rawHeight(rawHeight),
m_xMultiplier(heightMap_x),
m_zMultiplier(heightMap_z),
m_yMultiplier(heightMap_y),
m_xTexCoord(heightMap_tex_x),
m_zTexCoord(heightMap_tex_z),
m_noiseGenerator(perlinNoise){
	m_position = Vector3(0, 0, 0);





}

HeightMap::~HeightMap(){
	
}


void HeightMap::generateFlatTerrain(){
	
	m_numVertices	= m_rawWidth * m_rawHeight;
	m_numIndices	= (m_rawWidth - 1) * (m_rawHeight - 1) * 6;
	
	m_vertices		= new Vector3[m_numVertices];
	m_textureCoords = new Vector2[m_numVertices];
	m_indices		= new GLuint[m_numIndices];
	
	for (int x = 0; x < m_rawWidth; ++x) {
		for (int z = 0; z < m_rawHeight; ++z) {
			
			int offset				= (x* m_rawWidth) + z;
			m_vertices[offset]		= Vector3(x * m_xMultiplier, 0 , z * m_zMultiplier);
			m_textureCoords[offset] = Vector2(x * m_xTexCoord, z * m_zTexCoord);
		}
	}
	
	m_numIndices = 0;
	
	for (int x = 0; x < m_rawWidth - 1; ++x) {
		for (int z = 0; z < m_rawHeight - 1; ++z) {
			
			
			int a = (x		* (m_rawWidth)) + z;
			int b = ((x + 1)* (m_rawWidth)) + z;
			int c = ((x + 1)* (m_rawWidth)) + (z + 1);
			int d = (x		* (m_rawWidth)) + (z + 1);
			
			
			m_indices[m_numIndices++] = c;
			m_indices[m_numIndices++] = b;
			m_indices[m_numIndices++] = a;
			
			m_indices[m_numIndices++] = a;
			m_indices[m_numIndices++] = d;
			m_indices[m_numIndices++] = c;
			
		}
		
	}
}


void HeightMap::generateRandomTerrain(Vector3 position,int octaves, float frequency, float persistance, float min, float max) {
	m_numVertices = m_rawWidth * m_rawHeight;
	m_numIndices = (m_rawWidth - 1) * (m_rawHeight - 1) * 6;
	
	m_vertices		= new Vector3[m_numVertices];
	m_textureCoords = new Vector2[m_numVertices];
	m_indices		= new GLuint[m_numIndices];
	
	m_noiseGenerator->setDestScale(m_rawWidth);
	
	
	float scaledX = position.x / m_xMultiplier;
	float scaledZ = position.z / m_zMultiplier;
	
	m_position = position;
	for (int x = 0; x < m_rawWidth; ++x) {
		for (int z = 0; z < m_rawHeight; ++z) {

			int offset = (x* m_rawWidth) + z;
			
			float tempX = scaledX + x;
			float tempZ = scaledZ + z;
			
			Vector2 point(tempX,tempZ);
			
			
			float noise = m_noiseGenerator->noiseAt(point, octaves, frequency, persistance);
			
			
			
			if (noise < min) {
				noise = min;
			}

			if (noise > max) {
				noise = max;
			}
		
			m_vertices[offset]		= Vector3(x * m_xMultiplier, noise * m_yMultiplier, z * m_zMultiplier);
			m_textureCoords[offset]	= Vector2(x * m_xTexCoord, z * m_zTexCoord);
			
		}
	}
	
	
	m_numIndices = 0;
	
	for (int x = 0; x < m_rawWidth - 1; ++x) {
		for (int z = 0; z < m_rawHeight - 1; ++z) {
			
			
			int a = (x		* (m_rawWidth)) + z;
			int b = ((x + 1)* (m_rawWidth)) + z;
			int c = ((x + 1)* (m_rawWidth)) + (z + 1);
			int d = (x		* (m_rawWidth)) + (z + 1);
			
			
			m_indices[m_numIndices++] = c;
			m_indices[m_numIndices++] = b;
			m_indices[m_numIndices++] = a;
			
			m_indices[m_numIndices++] = a;
			m_indices[m_numIndices++] = d;
			m_indices[m_numIndices++] = c;
			
		}
		
	}
}


void HeightMap::updateTerrain(PerlinNoise3D* perlin3D,Vector3 position,int octaves, float frequency, float persistance){
	if(!m_vertices){
		std::cout << "There is no terrain to update!" <<std::endl;
		return;
	}
	
	float scaledX = position.x / m_xMultiplier;
	float scaledZ = position.z / m_zMultiplier;
	
	for (int x = 0; x < m_rawWidth; ++x) {
		for (int z = 0; z < m_rawHeight; ++z) {
			int offset = (x* m_rawWidth) + z;
			
			float tempX = scaledX + x;
			float tempZ = scaledZ + z;
			
			Vector3 tempPoint(tempX,tempZ,position.z);

			float noise = perlin3D->noiseAt(tempPoint, octaves, frequency, persistance);


			m_vertices[offset]		= Vector3(x * m_xMultiplier, noise * m_yMultiplier, z * m_zMultiplier);
			m_textureCoords[offset]	= Vector2(x * m_xTexCoord, z * m_zTexCoord);
			
		}
	}
	
	m_numIndices = 0;
	for (int x = 0; x < m_rawWidth - 1; ++x) {
		for (int z = 0; z < m_rawHeight - 1; ++z) {
			
			
			int a = (x		* (m_rawWidth)) + z;
			int b = ((x + 1)* (m_rawWidth)) + z;
			int c = ((x + 1)* (m_rawWidth)) + (z + 1);
			int d = (x		* (m_rawWidth)) + (z + 1);
			
			
			m_indices[m_numIndices++] = c;
			m_indices[m_numIndices++] = b;
			m_indices[m_numIndices++] = a;
			
			m_indices[m_numIndices++] = a;
			m_indices[m_numIndices++] = d;
			m_indices[m_numIndices++] = c;
		}
	}
	updateData();
}










