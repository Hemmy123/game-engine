#pragma once
#include "Renderer.h"


class DeferredRenderer
{
public:
	DeferredRenderer(Renderer * parentRenderer, Mesh* quad);
	~DeferredRenderer();

	// Rendering
	void renderScene(GLuint FBO);

	// Setting up
	void createLights();
	void initBuffers();
	void generateBuffers();

	void generateAllTextures();
	void generateScreenTexture(GLuint &into, bool depth = false);
	void attachTextures();

	// Rendering 
	void fillBuffers();
	void drawLights();
	void combineBuffers(GLuint sceneFBO);

	GLuint getCombineFBO()		const { return m_combinedFBO; }
	GLuint getCombinedColour()	const { return m_combinedColour; }

private:


	// Temp array of lights for testing;
	const int rowLenth = 8;

	Light* m_lights;
	Mesh* m_quad;

	Shader* m_sceneShader;
	Shader* m_lightShader;
	Shader* m_combineShader;

	Renderer* m_parentRenderer;

	GLuint m_Gbuffer;
	GLuint m_GColour;
	GLuint m_GDepth;
	GLuint m_GNormal;

	GLuint m_lightFBO;
	GLuint m_lightEmissive;
	GLuint m_lightSpecular;

	// The final combined to be used
	// by other processing such as 
	// combining with a cubemap or
	// post processing
	GLuint m_combinedFBO;
	GLuint m_combinedColour;

};

