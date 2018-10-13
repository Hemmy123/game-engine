////========================================
// Class: GameObject
// Author: Hemmy
// Date: 11/07/2018
// Description:
//
// ========================================



#pragma once
#include <stdio.h>
#include "Vector3.h"
#include "Matrix4.h"


// Quick and easy tag which defines
// what a gameobject this. This will be
// turned into a proper data driven method
// later as this hard coded tag method
// is rather messy
enum ObjectTag{
	T_Player,
	T_Wall,
	T_Platform,
	T_Trigger,
	
	
	// Test Tags
	
	T_Rabbit,
	T_Terrain,
	T_Water,
	T_Cube
	
};

//#pragma once
//
//// The base data required for a game object.
//// This can be used by other subsystems like
//// graphics, physics and audio for positioning
//// data
//struct {
//	char* m_UID;
//	
//	Vector3 m_position;
//	Vector3 m_rotation; // Change to quaternion?
//	Vector3 m_scale;
//	
//	Matrix4 m_modelMatrix;
//	
//}GameObjectData;

#pragma once

class GameObject {
public:
	GameObject();
	GameObject(Vector3 pos, Vector3 rot, Vector3 sca);
	
	~GameObject();
	
	
	// ---------- Getters/Setters ---------- //
	inline Vector3 getPosition() 	const { return m_position;}
	inline Vector3 getRotation() 	const { return m_rotation;}
	inline Vector3 getScale() 		const { return m_scale;}
	inline ObjectTag getTag()		const { return m_tag;}

	inline void setTag(ObjectTag t)		{ m_tag = t;}
	inline void setPosition(Vector3 p) 	{ m_position = p;};
	inline void setRotation(Vector3 r) 	{ m_rotation = r;};
	inline void setScale(Vector3 s) 	{ m_scale = s;};
	
protected:
	
	char* m_UID;
	
	Vector3 m_position;
	Vector3 m_rotation; // Change to quaternion?
	Vector3 m_scale;
	
	Matrix4 m_modelMatrix;
	
	// What texture/mesh the object should have.
	ObjectTag m_tag;
	
	
};


