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




class KeyboardInterface {
public:
	KeyboardInterface(GLFWwindow *windowListener);
	~KeyboardInterface();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void update();
	

	static void handleSinglePress(int key, int action);

	static void handleHeldPress(int key, int action);
	static void handleRelease(int key, int action);

	bool* getPressedKeys() const { return m_pressedKeys; }
	bool* getHeldHeys() const { return m_heldKeys; }

	static bool m_pressedKeys[GLFW_KEY_LAST];
	static bool m_heldKeys[GLFW_KEY_LAST];


private:
	GLFWwindow *m_windowListener;
};


