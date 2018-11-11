#pragma once
#include "RenderObject.h"
#include "HeightMap.h"
using std::vector;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void pushRenderObject(RenderObject* ro);
	inline void pushLight(Light* light)		{ m_lights.push_back(light); };
	inline void setWater(RenderObject* w)	{ m_water = w; };

	inline RenderObject* getWater() const	{ return m_water; };
	vector<RenderObject*> getTransparent()	{ return m_transparent; };
	vector<RenderObject*> getOpaque()		{ return m_opaque; };
	vector<Light*> getLights()				{ return m_lights; };
private:
	vector<RenderObject*>	m_transparent;
	vector<RenderObject*>	m_opaque;
	vector<Light*>			m_lights;

	RenderObject* m_water;

};

