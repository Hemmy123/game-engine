////========================================
// Class: Light
// Author: Hemmy
// Date: 08/07/2018
// Description:
//
// ========================================
#pragma once


#include <stdio.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"

class Light {
	
public:
	
	/// Created a single coloured light (Ambient, Diffuse and Specular all the same)
	/// with a position and radius
	Light(Vector3 position, Vector4 colour, float radius):m_position(position), m_colour(colour), m_radius(radius){}
	Light();
	
	~Light() {};
	
	// ---------- Getters / Setters ---------- //
	Vector3 getPosition() 	const { return m_position;}
	Vector4 getColour() 	const { return m_colour;}
	float 	getRadius() 	const { return m_radius;}
	Mesh*	getMesh()		const { return m_mesh;}
	
	void setPosition(Vector3 position) 	{ m_position = position;}
	void setColour(Vector4 colour) 		{ m_colour 	= colour;}
	void setRadius(float radius) 		{ m_radius 	= radius;}
	void setMesh(Mesh* mesh)			{ m_mesh = mesh; }

protected:
	Vector3 m_position;
	Vector4 m_colour;
	float	m_radius;

	// in case we wanted to render a 
	// circle to represent the light
	Mesh*	m_mesh;
	
};

 /* Light_hpp */
