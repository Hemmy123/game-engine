#pragma once
#include "Shader.h"

class PerlinShaderInterface
{
public:
	PerlinShaderInterface();
	~PerlinShaderInterface();

	void updateUniforms();
	void generate1DTexture();


	void setShader(Shader* shader) { m_perlinShader = shader; }

private:
	Shader* m_perlinShader;
	static int const permSize = 256;

	int permArray[permSize];

};

