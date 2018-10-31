#pragma once
#include "Renderer.h"


class PostProcessor
{
public:
	PostProcessor(Renderer* r, Mesh* screenQuad);
	~PostProcessor();


	void generateFBOTexture();

	void drawPostProcess(GLuint buffColourAttachment);


private:

	Renderer* m_parentRenderer;

	// ----- Meshes/Shaders ----- //
	Mesh*	m_screenQuad;			// Quad for rendering FBO to screen
	Shader* m_processShader;	// For any post processing effects

	// ----- FBOs ----- //
	GLuint m_processFBO;


};

