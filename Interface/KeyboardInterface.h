////========================================
// Class: KeyboardInterface
// Author: Hemmy
// Date: 16/06/2018
// Description:
//
// Basic keyboard input. If everything is static
// that means I haven't added functionality for
// different windows yet. That's on the TODO list!
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

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void update();
	

	static void handleSinglePress(int key, int action);

	static void handleHeldPress(int key, int action);

	KeyState getKeyState() const {return m_keystate; }
	
	bool* getPressedKeys() const { return m_pressedKeys; }

	static bool m_pressedKeys[GLFW_KEY_LAST];
	static bool m_heldKeys[GLFW_KEY_LAST];


private:


	KeyState m_keystate;
	GLFWwindow *m_windowListener;
};


