#pragma once
#include "GameObject.h"

class GameHeightMap : GameObject{
public:
	GameHeightMap(Vector3 pos, Vector3 rot, Vector3 sca);
	~GameHeightMap();



	void setX(float x) { m_heightMap_x = x; };
	void setY(float y) { m_heightMap_z = y; };
	void setZ(float z) { m_heightMap_y = z; };
	void setRawWidth(float w) { m_rawWidth = w;};
	void setRawHeight(float h) { m_rawHeight = h; };

private:
	int m_rawWidth = 257;
	int m_rawHeight = 257;
	float m_heightMap_x = 1;
	float m_heightMap_z = 1;
	float m_heightMap_y = 10;
	float m_heightMap_tex_x = 1 / m_heightMap_x;
	float m_heightMap_tex_z = 1 / m_heightMap_z;
};

