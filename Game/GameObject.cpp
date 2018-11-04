////========================================
// Class: GameObject
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================

#include "GameObject.h"


GameObject::GameObject():
	m_position(Vector3(0, 0, 0)),
	m_rotation(Vector3(1,1,1)),
	m_scale(Vector3(1,1,1))

{
	//TODO: Make UUID here
		

}

GameObject::GameObject(Vector3 pos, Vector3 rot, Vector3 sca):
m_position(pos),
m_rotation(rot),
m_scale(sca){

	Matrix4 posMatrix = Matrix4::Translation(pos);
	//Matrix4 rot = Matrix4::Rotation(pos);
	Matrix4 scaleMatrix = Matrix4::Scale(sca);

	//TODO: Need to do rotation here!!
	m_modelMatrix = posMatrix * scaleMatrix;
	
}

GameObject::~GameObject(){
	// Free/Delete UUID
}


