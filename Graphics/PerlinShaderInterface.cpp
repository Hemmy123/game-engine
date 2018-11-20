#include "PerlinShaderInterface.h"

#include <stdio.h>
#include <numeric>
#include <vector>
#include <random>
#include <algorithm>


PerlinShaderInterface::PerlinShaderInterface()
{
	generatePermArray(10);
	height = 10;
	octaves = 3;
	frequency = 10;
	persistance = 0.5;
	perlinTime = 10;

	generate1DTexture();

}


PerlinShaderInterface::~PerlinShaderInterface()
{
}

void PerlinShaderInterface::updateUniforms()
{		

	glUseProgram(m_perlinShader->getProgram());
	GLuint heightLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "height");
	GLuint permSizeLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "permutationSize");

	GLuint octavesLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "octaves");
	GLuint frequencyLoc		= glGetUniformLocation(m_perlinShader->getProgram(), "frequency");
	GLuint persistenceLoc	= glGetUniformLocation(m_perlinShader->getProgram(), "persistance");
	GLuint perlinTimeLoc	= glGetUniformLocation(m_perlinShader->getProgram(), "perlinTime");
	GLuint permTextureLoc	= glGetUniformLocation(m_perlinShader->getProgram(), "permTexture");


	glUniform1i(octavesLoc, octaves);
	glUniform1f(frequencyLoc, frequency);
	glUniform1f(persistenceLoc, persistance);
	glUniform1f(perlinTimeLoc, perlinTime);
	glUniform1i(permTextureLoc, TextureUniforms::PermArray);


	glActiveTexture(GL_TEXTURE0 + TextureUniforms::PermArray);
	glBindTexture(GL_TEXTURE_2D, m_permTexture);



}

void PerlinShaderInterface::generate1DTexture()
{
	GLenum error = glGetError();

	glGenTextures(1, &m_permTexture);
	error = glGetError();
	glBindTexture(GL_TEXTURE_1D, m_permTexture);

	error = glGetError();
	glTexImage1D(
		GL_TEXTURE_1D,	// Target
		0,				// mipmap levels
		GL_R32UI,		// internal format (32bit uint)
		sizeof(int) * permSize,		// 'width' of the texture
		0,				// border? must be 0 apparently
		GL_R32UI,		// format of the pixel data
		GL_UNSIGNED_INT,	// data type of pixel data
		m_permArray);		// the actual data

	error = glGetError();
	glBindTexture(GL_TEXTURE_2D, 0);

	error = glGetError();
}

void PerlinShaderInterface::generatePermArray(int seed)
{
	for (int i = 0; i < permSize; i ++) {
		m_permArray[i] = rand() % permSize;
	}


}
