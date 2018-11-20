#pragma once
#include "RenderObject.h"
#include "HeightMap.h"
#include "Level.h"
using std::vector;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void pushRenderObject(RenderObject* ro);
	inline void pushLight(Light* light)		{ m_lights.push_back(light); };
	inline void setWater(RenderObject* w)	{ m_water = w; };
	inline void setSettings(RendererSettings settings) { m_settings = settings; };


	inline RenderObject* getWater() const	{ return m_water; };
	vector<RenderObject*> getTransparent()	{ return m_transparent; };
	vector<RenderObject*> getOpaque()		{ return m_opaque; };
	vector<Light*> getLights()				{ return m_lights; };
	RendererSettings getSettings()	const	{ return m_settings; };

	void clearAllObjects();

	void addScene(RenderObject* object);

	//void sortOpaque();
	//void sortTransparent();

private:
	vector<RenderObject*>	m_transparent;
	vector<RenderObject*>	m_opaque;
	vector<Light*>			m_lights;

	RenderObject* m_water;
	RendererSettings m_settings;
};

