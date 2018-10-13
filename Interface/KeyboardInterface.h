////========================================
// Class: KeyboardInterface
// Author: Hemmy
// Date: 16/06/2018
// Description:
//
// Very very basic keyboard input handling.
// Can only handle on button at a time :(,
// but its enough to move a camera and get
// testing going
//
// ========================================
#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct KeyState{
	
	KeyState(int key, int action, int mod):
	m_key(key),
	m_action(action),
	m_mod(mod) {};
	
	KeyState(void):
	m_key(GLFW_KEY_UNKNOWN),
	m_action(GLFW_KEY_UNKNOWN),
	m_mod(GLFW_KEY_UNKNOWN) {};
	
	int m_key;
	int m_action;
	int m_mod;
	
};

class KeyboardInterface {
public:
	KeyboardInterface(GLFWwindow *windowListener);
	~KeyboardInterface();
	void update();
	
	KeyState getKeyState() const {return m_keystate; }
	
private:
	KeyState m_keystate;
	GLFWwindow *m_windowListener;
};


