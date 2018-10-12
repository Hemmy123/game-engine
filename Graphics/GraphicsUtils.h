////========================================
// Class: GraphicsUtils
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================
#pragma once


#include <stdio.h>
#include <vector>
#include "GameObject.h"
class GraphicsUtils {
public:
	
	static void gameObjVecToRenderObj(std::vector<GameObject*> const objs);
	
	
	/// Turns a single GameObject into a RenderObject to
	static void gameObjToRenderObj(GameObject const &gameObj);

	
private:
	

};


 /* GraphicsUtils_hpp */
