#pragma once
#include "Shader.h"
#include "FilePaths.h"
class Shadows
{
public:
	Shadows();
	~Shadows();


private:

	GLuint m_shadowFBO;
	GLuint m_shadowTex;

	Shader* m_shadowShader;
	Shader* m_sceneShader;
	Light*	m_light;
};

