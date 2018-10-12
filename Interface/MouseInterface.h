////========================================
// Class: MouseInterface
// Author: Hemmy
// Date: 19/06/2018
// Description:
// Handles communication between the mouse
// and the computer using GLFW
// ========================================


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector2.h"

struct MouseButtonState{
	MouseButtonState(int button, int action, int mods):
	m_button(button),
	m_action(action),
	m_mods(mods) {};
	
	MouseButtonState(void):
	m_button(GLFW_KEY_UNKNOWN),
	m_action(GLFW_KEY_UNKNOWN),
	m_mods(GLFW_KEY_UNKNOWN) {};
	
	int m_button;
	int m_action;
	int m_mods;
	
};


class MouseInterface {
public:
	MouseInterface(GLFWwindow* window);
	~MouseInterface();

	void update();
	void getPressedKey();
	
	void calculateRelativePosition();
	void calculateAbsolutePosition();
	
	Vector2 getAbsolutePosition() const { return m_absPosition;}
	Vector2 getRelativePosition() const { return m_relativePosition;}
	
	void setSensitivity(float s) { m_sensitivity = s; }
	
	
private:
	
	MouseButtonState m_keyState;
	int m_pressedKey;
	
	GLFWwindow* m_windowListener;
	
	// Curser positions
	Vector2 m_absPosition;		// absoluate position this frame
	Vector2 m_relativePosition;	// relative position this frame
	Vector2 m_lastAbsolute;		// The absoluate position of the mouse last frame
	
	float m_sensitivity = 0.1f;
};

