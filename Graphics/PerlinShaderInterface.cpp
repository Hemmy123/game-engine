#include "PerlinShaderInterface.h"



PerlinShaderInterface::PerlinShaderInterface()
{
}


PerlinShaderInterface::~PerlinShaderInterface()
{
}

void PerlinShaderInterface::updateUniforms()
{
	GLuint heightLoc = glGetUniformLocation(m_perlinShader->getProgram(), "height");
	GLuint permSizeLoc = glGetUniformLocation(m_perlinShader->getProgram(), "permutationSize");

	GLuint octavesLoc = glGetUniformLocation(m_perlinShader->getProgram(), "octaves");
	GLuint frequencyLoc = glGetUniformLocation(m_perlinShader->getProgram(), "frequency");
	GLuint persistenceLoc = glGetUniformLocation(m_perlinShader->getProgram(), "persistance");
	GLuint perlinTimeLoc = glGetUniformLocation(m_perlinShader->getProgram(), "perlinTime");
	GLuint permLoc = glGetUniformBlockIndex(m_perlinShader->getProgram(), "PermBlock");


}

void PerlinShaderInterface::generate1DTexture()
{
	glTexImage1D(
		GL_TEXTURE_1D,	// Target
		0,				// mipmap levels
		GL_R32UI,		// internal format (32bit uint)
		permSize,		// 'with' of the texture
		0,				// border? must be 0 apparently
		GL_R32UI,		// format of the pixel data
		GL_UNSIGNED_INT,	// data type of pixel data
		permArray);		// the actual data
}
