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


	handleHeldPress(key, action);
	handleSinglePress(key, action);

	


}

void KeyboardInterface::update(){
	/*if (glfwGetKey( m_windowListener, GLFW_KEY_W ) == GLFW_PRESS){
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
	}*/

	
}

void KeyboardInterface::handleSinglePress(int key, int action)
{

	bool lastState = KeyboardInterface::m_pressedKeys[key];

	if (action == GLFW_PRESS) {

		KeyboardInterface::m_pressedKeys[key] = true;
		std::cout << "1 pressed" << std::endl;
	}
	else if (action == GLFW_RELEASE) {
		KeyboardInterface::m_pressedKeys[key] = false;

	}

}

void KeyboardInterface::handleHeldPress(int key, int action)
{

}




