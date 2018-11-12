#include "SceneManager.h"



SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	clearAllObjects();
}

void SceneManager::pushRenderObject(RenderObject * ro)
{
	if (ro->getTransparent()) {
		m_transparent.push_back(ro);
	}
	else {
		m_opaque.push_back(ro);
	}
}

void SceneManager::clearAllObjects()
{
	for (auto obj : m_transparent) {
		delete obj;
	}

	for (auto obj : m_opaque) {
		delete obj;
	}

	for (auto obj : m_lights) {
		delete obj;
	}

	if (m_water) {
		delete m_water;
	}


}
