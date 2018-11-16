#include "DeferredRenderer.h"


DeferredRenderer::DeferredRenderer()
{
}


DeferredRenderer::~DeferredRenderer()
{
	glDeleteFramebuffers(1,&m_Gbuffer);
	glDeleteTextures(1,&m_GColour);
	glDeleteTextures(1,&m_GDepth);
	glDeleteTextures(1,&m_GNormal);

	glDeleteFramebuffers(1,&m_lightFBO);
	glDeleteTextures(1,&m_lightEmissive);
	glDeleteTextures(1,&m_lightSpecular);




}

void DeferredRenderer::initBuffers()
{
	generateBuffers();
	generateAllTextures();
	attachTextures();
}

void DeferredRenderer::createLights()
{
	m_lights = new Light[rowLenth * rowLenth];

	for (int x = 0; x < rowLenth; ++x) {	

		for (int x = 0; x < rowLenth; ++x) {
		
		}
	}


}

void DeferredRenderer::generateBuffers()
{
	glGenFramebuffers(1, &m_Gbuffer);
	glGenFramebuffers(1, &m_lightFBO);
}

void DeferredRenderer::generateAllTextures()
{
	

	generateScreenTexture(m_GDepth, true);
	generateScreenTexture(m_GColour);
	generateScreenTexture(m_GNormal);
	generateScreenTexture(m_lightEmissive);
	generateScreenTexture(m_lightSpecular);

	
}

void DeferredRenderer::generateScreenTexture(GLuint & into, bool depth)
{
	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();
	
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	


	glTexImage2D(GL_TEXTURE_2D, 0,
		depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8,
		width, height, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);


}

void DeferredRenderer::attachTextures()
{
	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	// Set up Gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_Gbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GColour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GDepth, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GNormal, 0);

	// Which buffers to draw to.
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;

	}

	// Set up light buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightEmissive, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightEmissive, 0);
	glDrawBuffers(2, buffers);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);


}

void DeferredRenderer::fillBuffers()
{
	// Bind the G buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_Gbuffer);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_parentRenderer->setCurrentShader(m_sceneShader);

	GLuint diffuseLoc	= glGetUniformLocation(m_sceneShader->getProgram(), "diffuseTex");
	GLuint bumpLoc		= glGetUniformLocation(m_sceneShader->getProgram(), "bumpTex");
	
	glUniform1i(diffuseLoc, TextureUniforms::Diffuse);
	glUniform1i(bumpLoc, TextureUniforms::Bump);

	m_parentRenderer->changeProjection(Projection::Perspective);
	// Change model matrix here?
	m_parentRenderer->updateShaderMatrices();

	// draw stuff here


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::drawLights()
{
	// Bind our lighting FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	m_parentRenderer->setCurrentShader(m_lightShader);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Check how this blending works?
	glBlendFunc(GL_ONE, GL_ONE);
	
	GLuint depthTexLoc	= glGetUniformLocation(m_lightShader->getProgram(), "depthTex");
	GLuint normTexLoc	= glGetUniformLocation(m_lightShader->getProgram(), "normTex");


	glUniform1i(depthTexLoc, Depth);
	glUniform1i(normTexLoc, Normal);


//	glActiveTexture(GL_TEXTURE)

}

