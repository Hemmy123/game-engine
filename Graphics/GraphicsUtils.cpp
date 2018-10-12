////========================================
// Class: GraphicsUtils
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================

#include "GraphicsUtils.h"
#include "RenderObject.h"

void GraphicsUtils::gameObjToRenderObj(const GameObject &gameObj){
	
	RenderObject* ro = new RenderObject();

	
	float rotationDegree = gameObj.getRotation().Length();
	Vector3 rotDir = gameObj.getRotation().Normalise();	// I think?
	
	Matrix4 pos = Matrix4::Translation(gameObj.getPosition());
	Matrix4 rot	= Matrix4::Rotation(rotationDegree, rotDir);
	Matrix4 sca = Matrix4::Scale(gameObj.getScale());
	
	// Scale, then rotation and position
	Matrix4 modelMatix = pos * rot * sca;
	
	ro->setModelMatrix(modelMatix);
	
	switch(gameObj.getTag()){
		case (T_Player):{
			
			break;
		}
		case(T_Wall):{
			
			break;
		}
		default:{
			
		}
	}
	
	ro->setTransparent(false);
	
	delete ro;
	
}
