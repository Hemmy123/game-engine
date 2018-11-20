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
		m_water = nullptr;
	}

	m_transparent.clear();
	m_opaque.clear();
	m_lights.clear();

	

}

void SceneManager::addScene(RenderObject * object)
{
	vector<RenderObject*> children = object->getChildren();
	for (auto child : children) {
		addScene(child);
	}

	if (!object->getMesh()) {
		return;
	}

	if (object->getTransparent()) {
		m_transparent.push_back(object);
	}
	else {
		m_transparent.push_back(object);
	}


}
