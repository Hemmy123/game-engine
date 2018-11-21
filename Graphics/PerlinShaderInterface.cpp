#include "PerlinShaderInterface.h"

#include <stdio.h>
#include <numeric>
#include <vector>
#include <random>
#include <algorithm>


PerlinShaderInterface::PerlinShaderInterface()
{
	generatePermArray(10);
	height = 2;
	octaves = 3;
	frequency = 10;
	persistance = 0.5;
	perlinTime = 0.0f;	// needs to actually update!

	generate1DTexture();

}


PerlinShaderInterface::~PerlinShaderInterface()
{
}

void PerlinShaderInterface::updateUniforms(float msec)
{		

	perlinTime += msec/20;

	GLuint heightLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "height");
	glUniform1f(heightLoc, height);
	error = glGetError();

	GLuint permSizeLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "permutationSize");
	glUniform1i(permSizeLoc, permSize);
	error = glGetError();

	GLuint octavesLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "octaves");
	glUniform1i(octavesLoc, octaves);
	error = glGetError();

	GLuint frequencyLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "frequency");
	glUniform1f(frequencyLoc, frequency);
	error = glGetError();

	GLuint persistenceLoc	= glGetUniformLocation(m_perlinShader->getProgram(), "persistance");
	glUniform1f(persistenceLoc, persistance);
	error = glGetError();

	GLuint perlinTimeLoc	= glGetUniformLocation(m_perlinShader->getProgram(), "perlinTime");
	glUniform1f(perlinTimeLoc, perlinTime);
	error = glGetError();

	GLuint permTextureLoc	= glGetUniformLocation(m_perlinShader->getProgram(), "permTexture");
	glUniform1i(permTextureLoc, TextureUniforms::PermArray);

	error = glGetError();

	glActiveTexture(GL_TEXTURE0 + TextureUniforms::PermArray);
	glBindTexture(GL_TEXTURE_1D, m_permTexture);
	error = glGetError();

}

void PerlinShaderInterface::generate1DTexture()
{

	glGenTextures(1, &m_permTexture);
	glBindTexture(GL_TEXTURE_1D, m_permTexture);


	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);

	glTexImage1D(
		GL_TEXTURE_1D,	// Target
		0,				// mipmap levels
		GL_R16UI,			// internal format
		permSize,		// 'width' of the texture
		0,				// border? must be 0 apparently
		GL_RED_INTEGER,			// format of the pixel data
		GL_INT,			// data type of pixel data
		&m_permArray);	// the actual data

	error = glGetError();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void PerlinShaderInterface::generatePermArray(int seed)
{
	for (int i = 0; i < permSize; i ++) {
		m_permArray[i] = (rand() % permSize);
	}


}
