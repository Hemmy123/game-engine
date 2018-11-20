#include "SceneManager.h"



SceneManager::SceneManager()
{
	m_cameraPos = Vector3(0, 0, 0);
}


SceneManager::~SceneManager()
{
	clearAllObjects();
}

void SceneManager::update(Vector3 cameraPos)
{
	m_cameraPos = cameraPos;
	updateDistancesFromCamera();

	sortRenderObjects();
}

void SceneManager::sortRenderObjects()
{
	std::sort(
		m_opaque.begin(),
		m_opaque.end(),
		SceneManager::compareByCameraDistance);

	std::sort(
		m_transparent.begin(),
		m_transparent.end(),
		SceneManager::compareByCameraDistance);


}

void SceneManager::updateDistancesFromCamera()
{
	for (auto obj : m_opaque) {
		Vector3 dir = obj->getWorldTransform().GetPositionVector() - m_cameraPos;
		// using squared disance to avoid squareroot from the actual distance
		obj->setDistanceFromCamera(Vector3::Dot(dir, dir));
	}

	for (auto obj : m_transparent) {
		Vector3 dir = obj->getWorldTransform().GetPositionVector() - m_cameraPos;
		obj->setDistanceFromCamera(Vector3::Dot(dir, dir));
	}


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

bool SceneManager::compareByCameraDistance(RenderObject * a, RenderObject * b)
{
	// return true if a is closer to the camera.
	return ( (a->getDistanceFromCamera() < b->getDistanceFromCamera() ) ? true : false);
}
