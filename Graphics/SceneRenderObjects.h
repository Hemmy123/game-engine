////========================================
// Class: SceneRenderObjects
// Author: Hemmy
// Date: 29/07/2018
// Description:
//
// ========================================
#pragma once

#include <stdio.h>
#include "RenderObject.h"
#include <vector>

using std::vector;
class SceneRenderObjects{
public:
	void setRenderObjects(vector<RenderObject*> ro);
	void addRenderObject(RenderObject *renderObject);
	
	vector<RenderObject*> getTransparentObjects() 	{ return m_transparentObjects;};
	vector<RenderObject*> getOpaqueObjects()		{ return m_opaqueObjects;};
	
	
private:
	
	vector<RenderObject*> m_transparentObjects;
	vector<RenderObject*> m_opaqueObjects;
	
	
};

 /* SceneRenderObjects_hpp */
