////========================================
// Class: MasterRenderer
// Author: Hemmy
// Date: 05/07/2018
// Description:
// Contains the base code to render something
// to the screen. If something fancy is needed
// like post-processing it can derive this class.
// (Similar to NCLGLs OGLRenderer)
//
//
// ========================================


#pragma once
#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <stdio.h>


#include "Matrix4.h"
#include "Shader.h"
#include "Light.h"

class MasterRenderer {
public:
	MasterRenderer(int height, int width) :HEIGHT(height), WIDTH(width) {};
	~MasterRenderer() {};

	virtual void update(float dt) = 0;
	virtual void renderScene() = 0;
	virtual void clearBuffers();


	void swapBuffers();




	// ---------- Getters/Setters ---------- //

	void setProjectionMatrix(Matrix4 proj) { m_projMatrix = proj; };
	void setViewMatrix(Matrix4 view) { m_viewMatrix = view; };
	void setOrthographicMatrix(Matrix4 ortho) { m_ortho = ortho; }
	void setPerspectiveMatrix(Matrix4 persp) { m_persp = persp; }

	Matrix4 getProjectionMatrix() 		const { return m_projMatrix; }
	Matrix4 getViewMatrix() 			const { return m_viewMatrix; }
	Matrix4 getOrthographicMatrix() 	const { return m_ortho; }
	Matrix4 getPerspectiveMatrix() 		const { return m_persp; }


protected:

	// ---------- Functions ---------- //

	/// Turns a glError enum into a string because OGL
	/// apparently doesn't have one of these :|
	std::string glEnumToString(unsigned e);

	/// Calls glGetError and then prints out the error.
	void checkErrors();



	int init();

	// ---------- Values ---------- //

	float m_dt;

	GLFWwindow *m_window;

	const GLint WIDTH;
	const GLint HEIGHT;

	int m_actualWidth;		// Dimensions reported by dimensional
	int m_actualHeight;

	float m_viewDistance = 1000;
	float m_fov = 45.0f;
	float m_aspectRatio;

	Matrix4 m_ortho;
	Matrix4 m_persp;

	Matrix4 m_projMatrix;
	Matrix4 m_modelMatrix;
	Matrix4 m_viewMatrix;
	Matrix4 m_textureMatrix;
};



