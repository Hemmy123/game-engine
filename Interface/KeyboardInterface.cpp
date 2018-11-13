////========================================
// Class: KeyboardInterface
// Author: Hemmy
// Date: 16/06/2018
// Description:
//
// ========================================

#include "KeyboardInterface.h"
#include <iostream>

bool KeyboardInterface::m_pressedKeys[GLFW_KEY_LAST];
bool KeyboardInterface::m_heldKeys[GLFW_KEY_LAST];


KeyboardInterface::KeyboardInterface(GLFWwindow *windowListener):m_windowListener(windowListener){
	
	glfwSetKeyCallback(windowListener, &key_callback);
}

KeyboardInterface::~KeyboardInterface(){
	
}

void KeyboardInterface::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	// Don't process unknown, keys
	if (key == GLFW_KEY_UNKNOWN) return;


	handleSinglePress(key, action);
	handleHeldPress(key, action);
	handleRelease(key, action);
	


}

void KeyboardInterface::update(){

	// Reset pressed keys
	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++) {
		m_pressedKeys[i] = false;
	}	
}

void KeyboardInterface::handleSinglePress(int key, int action)
{
	if (action == GLFW_PRESS) {
		KeyboardInterface::m_pressedKeys[key] = true;
	}

}

void KeyboardInterface::handleHeldPress(int key, int action)
{
	if (action == GLFW_PRESS) {
		KeyboardInterface::m_heldKeys[key] = true;
	}
}

void KeyboardInterface::handleRelease(int key, int action)
{
	if (action == GLFW_RELEASE) {
		KeyboardInterface::m_pressedKeys[key]	= false;
		KeyboardInterface::m_heldKeys[key]		= false;
	}


}




