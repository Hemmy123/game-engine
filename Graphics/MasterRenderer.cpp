////========================================
// Class: MasterRenderer
// Author: Hemmy
// Date: 05/07/2018
// Description:
//
// ========================================

#include "MasterRenderer.h"



int MasterRenderer::init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Trying setting opengl window to 3?
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // Setting core profile?

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // For running on Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Can't resize
	m_window = glfwCreateWindow(WIDTH, HEIGHT, "OpenglTest", nullptr, nullptr);

	glfwGetFramebufferSize(m_window, &m_actualWidth, &m_actualHeight); // have to do this because of retina mac scaling issue?
	if (m_window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(m_window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initlialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, m_actualWidth, m_actualHeight);


	// Cull faces we can't see
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Depth test so stuff doesn't render on top of each other;
	glEnable(GL_DEPTH_TEST);

	// Blend func for transparent objects;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);



	return 0;
}

void MasterRenderer::clearBuffers() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void MasterRenderer::swapBuffers() {
	glfwSwapBuffers(m_window);
}


void MasterRenderer::checkErrors() {
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "Something went wrong! " << glEnumToString(error) << std::endl;
	}


}



std::string MasterRenderer::glEnumToString(uint e) {

	std::string errStr = "Could not convert error enum to string!";
	switch (e) {
	case GL_NO_ERROR: {
		errStr = "GL_NO_ERROR";
		break;
	}
	case GL_INVALID_ENUM: {
		errStr = "GL_INVALID_ENUM";
		break;
	}
	case GL_INVALID_VALUE: {
		errStr = "GL_INVALID_VALUE";
		break;
	}
	case GL_INVALID_OPERATION: {
		errStr = "GL_INVALID_OPERATION";

	}
	case GL_INVALID_FRAMEBUFFER_OPERATION: {
		errStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	}
	case GL_OUT_OF_MEMORY: {
		errStr = "GL_OUT_OF_MEMORY";
		break;
	}
	case GL_STACK_UNDERFLOW: {
		errStr = "GL_STACK_UNDERFLOW";
		break;
	}
	case GL_STACK_OVERFLOW: {
		errStr = "GL_STACK_OVERFLOW";
		break;
	}

	}

	return errStr;


}




