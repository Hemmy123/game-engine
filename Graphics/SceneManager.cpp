#include "SceneManager.h"



SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
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
