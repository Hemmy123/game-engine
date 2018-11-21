////========================================
// Class: Camera
// Author: Hemmy
// Date: 09/06/2018
// Description:
//
// ========================================

#pragma once

#include <stdio.h>

#include "Vector3.h"
#include "Matrix4.h"

#include "InterfaceHandler.h"

class Camera {
	
public:
	Camera(InterfaceHandler* ih);
	
	Camera(InterfaceHandler* ih,float p, float y, Vector3 pos);
	
	~Camera(void) {};
	
	void	updateCamera(float msec);
	
	
	void 	update(float msec);

	
	

	Matrix4 BuildViewMatrix();
	
	Vector3 GetPosition() const { return m_position; }
	void	setPosition(Vector3 val) { m_position = val; }
	
	float	getYaw() const { return m_yaw; }
	void	setYaw(float y) { m_yaw = y; }
	
	float	getPitch()	const { return m_pitch; }
	void	setPitch(float p) { m_pitch = p; }
	
	float	getSpeed() const { return m_movementSpeed; }
	void	setSpeed(float s) { m_movementSpeed = s; }

	bool	getMoveable() const { return m_moveable; }
	void	setMoveable(bool b) { m_moveable = b; }

private:
	float m_dt;
	
	bool m_moveable;

	InterfaceHandler* m_interfaceHandler;
	GLFWwindow* m_windowListener;
	float m_yaw;			// Y (Left right)
	float m_pitch;			// X (Up Down)
	Vector3 m_position;
	float m_movementSpeed = 0.5;
};

 /* Camera_hpp */
