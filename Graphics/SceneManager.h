#pragma once
#include "RenderObject.h"
#include "HeightMap.h"
using std::vector;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();



	vector<RenderObject*> getTransparent()  { return m_transparent; };
	vector<RenderObject*> getOpaque()  { return m_opaque; };

	void pushRenderObject(RenderObject* ro);

	void setWater(HeightMap* w) { m_water = w; };

	HeightMap* getWater() { return m_water; };

private:
	vector<RenderObject*> m_transparent;
	vector<RenderObject*> m_opaque;

	HeightMap* m_water;

};

