#pragma once
#include "Renderer.h"


class PostProcessor
{
public:
	PostProcessor(Renderer* r);
	~PostProcessor();


	void generateFBOTexture();

	void drawSceneToFBO(GLuint fbo, Shader* shader);

	void drawPostProcess(GLuint fbo, Shader* shader);

	void presentScene();

private:

	Renderer* m_parentRenderer;

	// ----- Meshes/Shaders ----- //
	Mesh*	m_screenQuad;			// Quad for rendering FBO to screen
	Shader* m_sceneShader;		// For drawing the scene onto the quad
	Shader* m_processShader;	// For any post processing effects

	// ----- FBOs ----- //

	GLuint m_sceneFBO;
	GLuint m_processFBO;


	// ----- Frame Buffer Attachments ----- //
	GLuint m_buffColourAttachment;
	GLuint m_buffDepthAttachment;

};

