#pragma once
#include "Renderer.h"

#include <map>

enum PostProcessingEffect {
	None,
	Bloom,
	GaussianBlur,
	SineBlur,
	BrightnessFilter
};

class PostProcessor
{
public:
	PostProcessor(Renderer* r, Mesh* screenQuad);
	~PostProcessor();



	void drawPostProcessing(GLuint colourAttachment, PostProcessingEffect effect);


	void createShaders();


	/// This method will apply the post processing effect that has been created to the 
	/// quad. This is mainly here here for situations when there are a series of post 
	/// processing effects, (e.g. brightness detection, and blur for bloom) that all need
	/// to happen first before we actually apply them to the quad
	void drawFinalPostProcess(GLuint colourAttachment);

	/// Setting up FBOS
	void generateProcessFBO();
	void attachAttachments();

private:

	void drawPostProcessPingPong(GLuint colourAttachment, Shader* shader, int passes);
	
	/// Does one pass of post processing and places texture in 
	/// m_processColourAttachment[0]
	void drawPostProcess(GLuint colourAttachment, Shader* shader);


	std::map<PostProcessingEffect, Shader*> m_effectsMap;

	Renderer* m_parentRenderer;

	// ----- Meshes/Shaders ----- //
	Mesh*	m_screenQuad;			// Quad for rendering FBO to screen
	Shader* m_passThroughShader;
	Shader* m_combineShader;

	// ----- FBOs ----- //
	GLuint m_processFBO;

	GLuint m_processColourAttachment[2];	// two for ping pong
	GLuint m_processDepthAttachment;

	bool init;




};

