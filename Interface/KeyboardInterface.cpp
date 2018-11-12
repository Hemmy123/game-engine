////========================================
// Class: KeyboardInterface
// Author: Hemmy
// Date: 16/06/2018
// Description:
//
// ========================================

#include "KeyboardInterface.h"
#include <iostream>

KeyboardInterface::KeyboardInterface(GLFWwindow *windowListener):m_windowListener(windowListener){
	
//	glfwSetKeyCallback(windowListener, this->key_callback);
}

KeyboardInterface::~KeyboardInterface(){
	
}

void KeyboardInterface::update(){
	if (glfwGetKey( m_windowListener, GLFW_KEY_W ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_W, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}
	
	else if (glfwGetKey( m_windowListener, GLFW_KEY_A ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_A, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}
	
	else if (glfwGetKey( m_windowListener, GLFW_KEY_S ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_S, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}
	
	else if (glfwGetKey( m_windowListener, GLFW_KEY_D ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_D, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}
	
	else if (glfwGetKey( m_windowListener, GLFW_KEY_SPACE ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_SPACE, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}
	else if (glfwGetKey( m_windowListener, GLFW_KEY_C ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_C, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}

	else if (glfwGetKey(m_windowListener, GLFW_KEY_0) == GLFW_PRESS) {
		m_keystate = KeyState(GLFW_KEY_0, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}

	else if (glfwGetKey(m_windowListener, GLFW_KEY_1) == GLFW_PRESS) {
		m_keystate = KeyState(GLFW_KEY_1, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}

	else if (glfwGetKey(m_windowListener, GLFW_KEY_2) == GLFW_PRESS) {
		m_keystate = KeyState(GLFW_KEY_2, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}

	else if (glfwGetKey(m_windowListener, GLFW_KEY_3) == GLFW_PRESS) {
		m_keystate = KeyState(GLFW_KEY_3, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}

	else if (glfwGetKey( m_windowListener, GLFW_KEY_ESCAPE ) == GLFW_PRESS){
		m_keystate = KeyState(GLFW_KEY_ESCAPE, GLFW_PRESS, GLFW_KEY_UNKNOWN);
	}
	else{
		m_keystate = KeyState();
	}

	
}




