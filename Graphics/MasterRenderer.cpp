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

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // For running on Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Can't resize
	
	
	int numMonitors;
	GLFWmonitor **monitors = glfwGetMonitors(&numMonitors);

	std::string title("Graphics Engine");

	if (numMonitors > 1) {
		m_window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), monitors[1], nullptr);

	}
	else {
		m_window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), glfwGetPrimaryMonitor(), nullptr);

	}
	

	// Gets the width and height of the framebuffer being rendered 
	// and places it in m_actualWidth / m_actualHeight. Not really 
	// needed but handy for supporting higher res screens like 
	// the Apple Retina screens.
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



	return 0;
}

MasterRenderer::~MasterRenderer() {

}

void MasterRenderer::clearBuffers() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void MasterRenderer::swapBuffers() {
	glfwSwapBuffers(m_window);
}

void MasterRenderer::SetTextureRepeating(GLuint target, bool repeating)
{
	
		glBindTexture(GL_TEXTURE_2D, target);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
		glBindTexture(GL_TEXTURE_2D, 0);
	
}


void MasterRenderer::checkErrors() {
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "Something went wrong! " << glEnumToString(error) << std::endl;
	}


}



std::string MasterRenderer::glEnumToString(unsigned e) {

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
		break;
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


#ifdef OPENGL_DEBUGGING
void MasterRenderer::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, void* userParam) {

	string sourceName;
	string typeName;
	string severityName;

	switch (source) {
	case GL_DEBUG_SOURCE_API_ARB: sourceName = "Source(OpenGL)"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: sourceName = "Source(Window System)"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: sourceName = "Source(Shader Compiler)"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: sourceName = "Source(Third Party)"; break;
	case GL_DEBUG_SOURCE_APPLICATION_ARB: sourceName = "Source(Application)"; break;
	case GL_DEBUG_SOURCE_OTHER_ARB: sourceName = "Source(Other)"; break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR_ARB: typeName = "Type(Error)"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: typeName = "Type(Deprecated Behaviour)"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: typeName = "Type(Undefined Behaviour)"; break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB: typeName = "Type(Portability)"; break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB: typeName = "Type(Performance)"; break;
	case GL_DEBUG_TYPE_OTHER_ARB: typeName = "Type(Other)"; break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH_ARB: severityName = "Priority(High)"; break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB: severityName = "Priority(Medium)"; break;
	case GL_DEBUG_SEVERITY_LOW_ARB: severityName = "Priority(Low)"; break;
	}

	cout << "OpenGL Debug Output: " + sourceName + ", " + typeName + ", " + severityName + ", " + string(message) << endl;
}
#endif




