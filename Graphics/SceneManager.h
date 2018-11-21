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


	void update(Vector3 cameraPos);

	void sortRenderObjects();
	void updateDistancesFromCamera();

	void pushRenderObject(RenderObject* ro);
	inline void pushLight(Light* light)		{ m_lights.push_back(light); };
	inline void setWater(RenderObject* w)	{ m_water = w; };
	inline void setSettings(RendererSettings settings) { m_settings = settings; };
	inline void setSceneID(int i) { m_sceneID = i;  };

	inline RenderObject* getWater() const	{ return m_water; };
	inline vector<RenderObject*> getTransparent()	{ return m_transparent; };
	inline vector<RenderObject*> getOpaque()		{ return m_opaque; };
	inline vector<Light*> getLights()				{ return m_lights; };
	inline RendererSettings getSettings() const		{ return m_settings; };
	inline int getSceneID()	const					{ return m_sceneID; }
	void clearAllObjects();

	void addScene(RenderObject* object);


	static bool compareByCameraDistance(RenderObject* a, RenderObject* b);



private:
	int m_sceneID;

	Vector3 m_cameraPos;

	vector<RenderObject*>	m_transparent;
	vector<RenderObject*>	m_opaque;
	vector<Light*>			m_lights;

	RenderObject* m_water;
	RendererSettings m_settings;

	

};

