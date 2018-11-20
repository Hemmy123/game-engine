#pragma once
#include "Shader.h"

class PerlinShaderInterface
{
public:
	PerlinShaderInterface();
	~PerlinShaderInterface();

	void updateUniforms();
	void generate1DTexture();

	void generatePermArray(int seed);
	void setShader(Shader* shader) { m_perlinShader = shader; }

private:

	float height;
	int octaves;
	float frequency;
	float persistance;
	float perlinTime;

	Shader* m_perlinShader;
	static int const permSize = 256;
	
	GLuint m_permTexture;
	int m_permArray[permSize];

};

