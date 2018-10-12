////========================================
// Class: SceneRenderObjects
// Author: Hemmy
// Date: 29/07/2018
// Description:
//
// ========================================

#include "SceneRenderObjects.h"

void SceneRenderObjects::setRenderObjects(vector<RenderObject *> renderObjects){
	for(auto ro: renderObjects){
		addRenderObject(ro);
	}
	
}

void SceneRenderObjects::addRenderObject(RenderObject *renderObject){
	
	if(renderObject->getTransparent()){
		m_transparentObjects.push_back(renderObject);
	} else{
		m_opaqueObjects.push_back(renderObject);
	}
}
