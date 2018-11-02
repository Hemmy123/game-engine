#include "PostProcessor.h"

#include "FilePaths.h"

PostProcessor::PostProcessor(Renderer * r, Mesh* screenQuad):
	m_screenQuad(screenQuad),
	m_parentRenderer(r)
{
	m_processShader = new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Process_Frag.glsl");

	generateFBOTexture();
}

PostProcessor::~PostProcessor()
{

	delete m_processShader;

	glDeleteFramebuffers(1, &m_processFBO);
}

void PostProcessor::generateFBOTexture()
{
	glGenFramebuffers(1, &m_processFBO);


}

void PostProcessor::drawPostProcess(GLuint buffColourAttachment)
{

	// m_processFBO, m_processShader
	/*
	 Adds post processing to the scene.

	 Assuming the scene has been drawn to a FBO, this bindes the
	 processFBO, sets the texture to whatever was drawn before (so
	 the scene) and then applies post processing effect via a shader
	 by calling m_quad->draw()

	 Whatever is drawn is put into m_bufferColourAttachment (I think?)
	 */

	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();

	 // Bind the process FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_processFBO);
	glDisable(GL_DEPTH_TEST);

	m_parentRenderer->setCurrentShader(m_processShader);	// Change to our post processing shader
	m_parentRenderer->changeProjection(Orthographic);			
	m_parentRenderer->setViewMatrix(Matrix4());				// set to identitiy matrix
	m_parentRenderer->updateShaderMatrices();

	// Update screen size uniform.
	Shader* currentShader = m_parentRenderer->getCurrentShader();
	GLuint screenSizeLocation = glGetUniformLocation(currentShader->getProgram(), "screenSize");
	glUniform2f(screenSizeLocation, (width), (height));

	m_parentRenderer->checkErrors();


	GLuint timeID = glGetUniformLocation(currentShader->getProgram(), "time");
	glUniform1f(timeID, (float)(glfwGetTime()*10.0f));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffColourAttachment, 0);
	
	m_screenQuad->setTexture(buffColourAttachment);
	m_screenQuad->setTextureType(Texture_2D);
	m_screenQuad->bindTexture();
	m_screenQuad->draw();

	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
