#pragma once
#include "Renderer.h"

#include <map>

enum Effect {
	Bloom,
	GaussianBlur,
	SineBlur
};

class PostProcessor
{
public:
	PostProcessor(Renderer* r, Mesh* screenQuad);
	~PostProcessor();

	void drawPostProcessing(GLuint colourAttachment, Effect effect);

	void drawPostProcessPingPong(GLuint colourAttachment, Shader* shader, int passes);

	void createShaders();


	// Setting up FBOS
	void generateProcessFBO();
	void attachAttachments();

private:


	std::map<Effect, Shader*> m_effectsMap;

	const int POST_PASSES =10;

	Renderer* m_parentRenderer;

	// ----- Meshes/Shaders ----- //
	Mesh*	m_screenQuad;			// Quad for rendering FBO to screen

	// ----- FBOs ----- //
	GLuint m_processFBO;

	GLuint m_processColourAttachment;
	GLuint m_processDepthAttachment;

	bool init;




};

