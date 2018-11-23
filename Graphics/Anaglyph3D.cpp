#include "Anaglyph3D.h"

#include "FilePaths.h"



Anaglyph3D::Anaglyph3D(Renderer* r):
	m_parentRenderer(r)
{

	m_width = m_parentRenderer->getWidth();
	m_height = m_parentRenderer->getHeight();
	m_defaultShader = new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Scene_Frag.glsl");
	m_3DShader		= new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"3D_Frag.glsl");

	createFBOs();

	m_eyeDisplacement = Vector3(3, 0, 0);
}

Anaglyph3D::~Anaglyph3D(){
	delete m_defaultShader;
	glDeleteFramebuffers(1, &m_leftFBO);
	glDeleteFramebuffers(1, &m_rightFBO);
}

void Anaglyph3D::createFBOs()
{
	createLeftFBO();
	createRightFBO();
}

void Anaglyph3D::render(Mesh* quad,GLuint fbo, GLuint colourAttachment)
{
	renderRight();
	renderLeft();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	m_parentRenderer->setCurrentShader(m_3DShader);
	m_parentRenderer->updateShaderMatrices();

	GLuint leftTexloc = glGetUniformLocation(m_3DShader->getProgram(), "leftTex");
	GLuint rightTexLoc = glGetUniformLocation(m_3DShader->getProgram(), "rightTex");
	GLuint leftDepthLoc = glGetUniformLocation(m_3DShader->getProgram(), "leftDepthTex");
	GLuint rightDepthLoc = glGetUniformLocation(m_3DShader->getProgram(), "rightDepthTex");

	glUniform1i(leftTexloc, TextureUniforms::Left);
	glActiveTexture(GL_TEXTURE0 +TextureUniforms::Left);
	glBindTexture(GL_TEXTURE_2D, m_leftColourAttachment);

	glUniform1i(rightTexLoc, TextureUniforms::Right);
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::Right);
	glBindTexture(GL_TEXTURE_2D, m_rightColourAttachment);


	glUniform1i(leftDepthLoc, TextureUniforms::LeftDepth);
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::LeftDepth);
	glBindTexture(GL_TEXTURE_2D, m_leftDepthAttachment);


	glUniform1i(rightDepthLoc, TextureUniforms::RightDepth);
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::RightDepth);
	glBindTexture(GL_TEXTURE_2D, m_rightDepthAttachment);

	quad->draw();

	m_parentRenderer->checkErrors();


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Anaglyph3D::renderRight()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_rightFBO);
	// Only want blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rightColourAttachment, 0);
	

	Matrix4 oldViewMatrix = m_parentRenderer->getViewMatrix();


	Vector3 displacement(m_eyeDisplacement.x, 0, 0);

	Matrix4 newViewMatrix = oldViewMatrix * Matrix4::Translation(displacement);

	// set new view matrix
	m_parentRenderer->setViewMatrix(newViewMatrix);

	m_parentRenderer->setCurrentShader(m_defaultShader);
	m_parentRenderer->changeProjection(Projection::Perspective);
	m_parentRenderer->updateShaderMatrices();
	m_parentRenderer->drawAllRenderObjects();


	// set it back
	m_parentRenderer->setViewMatrix(oldViewMatrix);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Anaglyph3D::renderLeft()
{



	glBindFramebuffer(GL_FRAMEBUFFER, m_leftFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Only want red
	//glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_leftColourAttachment, 0);

	Matrix4 oldViewMatrix = m_parentRenderer->getViewMatrix();


	Vector3 displacement(-m_eyeDisplacement.x, 0, 0);

	Matrix4 newViewMatrix = oldViewMatrix * Matrix4::Translation(displacement);

	m_parentRenderer->setViewMatrix(newViewMatrix);
	// -----
	m_parentRenderer->setCurrentShader(m_defaultShader);
	m_parentRenderer->changeProjection(Projection::Perspective);
	m_parentRenderer->updateShaderMatrices();
	m_parentRenderer->drawAllRenderObjects();
	// -----

	m_parentRenderer->setViewMatrix(oldViewMatrix);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Anaglyph3D::createLeftFBO()
{

	// Depth Attachment
	glGenTextures(1, &m_leftDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_leftDepthAttachment);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	// Colour Attachment
	glGenTextures(1, &m_leftColourAttachment);
	glBindTexture(GL_TEXTURE_2D, m_leftColourAttachment);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Generate FBOs
	glGenFramebuffers(1, &m_leftFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_leftFBO);

	// Attaching attachments to sceneFBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_leftDepthAttachment, 0);		// Depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_leftDepthAttachment, 0);		// Stencil attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_leftColourAttachment, 0);		// Colour attachment

	// Checking if FBO attachment was successful
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !m_leftDepthAttachment || !m_leftColourAttachment) {
		std::cout << "FBO Attachment failed " << std::endl;
		m_parentRenderer->checkErrors();
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);


}

void Anaglyph3D::createRightFBO()
{
	// Depth Attachment
	glGenTextures(1, &m_rightDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_rightDepthAttachment);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	// Colour Attachment
	glGenTextures(1, &m_rightColourAttachment);
	glBindTexture(GL_TEXTURE_2D, m_rightColourAttachment);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Generate FBOs
	glGenFramebuffers(1, &m_rightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_rightFBO);

	// Attaching attachments to sceneFBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_rightDepthAttachment, 0);		// Depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_rightDepthAttachment, 0);		// Stencil attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rightColourAttachment, 0);		// Colour attachment

	// Checking if FBO attachment was successful
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !m_rightDepthAttachment || !m_rightColourAttachment) {
		std::cout << "FBO Attachment failed " << std::endl;
		m_parentRenderer->checkErrors();
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);

}
