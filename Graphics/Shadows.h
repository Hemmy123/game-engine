#pragma once
#include "Shader.h"
#include "Renderer.h"

int const SHADOWSIZE = 2048;
//int const SHADOWSIZE = 32;

class Shadows
{
public:
	Shadows(Renderer* parentRenderer);
	~Shadows();

	void drawShadowScene();
	void drawCombinedScene(GLuint sceneFbo);

	void drawScene(GLuint sceneFBO);

	int generateFBO();


private:

	GLuint m_shadowFBO;
	GLuint m_shadowTex;

	float count = 0;
	
	Renderer*	m_parentRenderer;
	Shader*		m_shadowShader;
	Shader*		m_sceneShader;
	Matrix4		m_biasMatrix;
	Matrix4		m_lightTextureMatrix;
	Light*		m_light;
};

