////========================================
// Class: MouseInterface
// Author: Hemmy
// Date: 19/06/2018
// Description:
//
// ========================================

#include "MouseInterface.h"
#include <iostream>

MouseInterface::MouseInterface(GLFWwindow* window):
m_keyState(),
m_windowListener(window),
m_absPosition(Vector2(0,0)),
m_relativePosition(Vector2(0,0)),
m_lastAbsolute(Vector2(0,0)){
	
	glfwSetInputMode(m_windowListener, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(m_windowListener, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
}

MouseInterface::~MouseInterface(){
	
	
}

void MouseInterface::update(){
	calculateAbsolutePosition();
	calculateRelativePosition();
	
	m_relativePosition  = m_relativePosition * m_sensitivity;
	
	getPressedKey();
	
	
}

void MouseInterface::getPressedKey(){
	int state = glfwGetMouseButton(m_windowListener, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){
		m_keyState = MouseButtonState(GLFW_MOUSE_BUTTON_LEFT,GLFW_PRESS,GLFW_KEY_UNKNOWN);
	}
	
}



void MouseInterface::calculateAbsolutePosition(){
	double x,y;
	glfwGetCursorPos(m_windowListener, &x, &y);
	
	
	//Save last position
	m_lastAbsolute = m_absPosition;
	
	// Update new position
	m_absPosition.x = (float)x;
	m_absPosition.y = (float)y;
	


	
}
	


void MouseInterface::calculateRelativePosition(){
	m_relativePosition = m_absPosition - m_lastAbsolute;
	
}
