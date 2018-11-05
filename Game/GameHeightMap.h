#pragma once
#include "GameObject.h"

class GameHeightMap : public GameObject{
public:
	GameHeightMap(Vector3 pos, Vector3 rot, Vector3 sca);
	~GameHeightMap();



	void setXMulti(float x) { m_x_multiplier = x; };
	void setYMulti(float y) { m_y_multiplier = y; };
	void setZMulti(float z) { m_z_multiplier = z; };
	void setRawWidth(int w) { m_rawWidth = w;};
	void setRawHeight(int h) { m_rawHeight = h; };

	int getRawWidth() const { return m_rawWidth; }
	int getRawHeight() const { return m_rawHeight; }
	float getXMultiplier() const { return m_x_multiplier; }
	float getYMultiplier() const { return m_y_multiplier; }
	float getZMultiplier() const { return m_z_multiplier; }
	float getTexCoordX() const { return m_heightMap_tex_x; }
	float getTexCoordZ() const { return m_heightMap_tex_z; }

private:
	int m_rawWidth = 257;
	int m_rawHeight = 257;
	float m_x_multiplier = 1;
	float m_z_multiplier = 1;
	float m_y_multiplier = 10;
	float m_heightMap_tex_x = 1 / m_x_multiplier;
	float m_heightMap_tex_z = 1 / m_z_multiplier;
};

