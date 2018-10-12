////========================================
// Class: PostProcessor
// Author: Hemmy
// Date: 29/07/2018
// Description:
//
// ========================================

#pragma once


#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Shader.h"
enum PostProcessingEffects{
	Post_Blurring_Y,
};


class PostProcessor{
	PostProcessor();
	~PostProcessor();
	
	void renderPostProcessing(bool trigger);
	
	
private:
	void generateFBOTexture();
	
	bool renderPostEffect;

	int m_height;
	int m_width;
	
	// -- Quad to render -- //
	Mesh*	m_quad;

	
	// -- Frame Buffer Objects -- //
	GLuint m_sceneFBO;			// FBO to represent the
	GLuint m_processFBO;
	
	
	// -- Frame Buffer Attachments -- //
	GLuint m_buffColourAttachment;
	GLuint m_buffDepthAttachment;
	
	// -- Shaders -- //
	Shader* m_sceneShader;		// For drawing the scene onto the quad
	Shader* m_processShader;	// For any post processing effects
};

 /* PostProcessor_hpp */
