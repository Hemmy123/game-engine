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

	// basic movelight method for testing moving shadows
	void moveLight();

	int generateFBO();

	void setLight(Light* l) { m_light = l; }
	Light* getLight() const { return m_light; }

private:

	GLuint m_shadowFBO;
	GLuint m_shadowTex;

	bool left	= false;
	bool right	= false;
	
	Renderer*	m_parentRenderer;
	Shader*		m_shadowShader;
	Shader*		m_sceneShader;
	Matrix4		m_biasMatrix;
	Matrix4		m_lightTextureMatrix;
	Light*		m_light;
};

