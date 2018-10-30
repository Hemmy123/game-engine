#include "PostProcessor.h"

#include "FilePaths.h"

PostProcessor::PostProcessor(Renderer * r):
	m_parentRenderer(r)
{

	m_sceneShader = new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Scene_Frag.glsl");
	m_processShader = new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Process_Frag.glsl");


	generateFBOTexture();

}

PostProcessor::~PostProcessor()
{

	delete m_sceneShader;
	delete m_processShader;
	delete m_screenQuad;

	glDeleteTextures(1, &m_buffColourAttachment);
	glDeleteTextures(1, &m_buffDepthAttachment);
	glDeleteFramebuffers(1, &m_sceneFBO);
	glDeleteFramebuffers(1, &m_processFBO);
}

void PostProcessor::generateFBOTexture()
{

	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();

	glGenTextures(1, &m_buffDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_buffDepthAttachment);
	m_parentRenderer->checkErrors();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	m_parentRenderer->checkErrors();

	// Note:
	// GL_DEPTH24_STENCIL8 and GL_DEPTH_STENCIL because it's a depth texture

	glGenTextures(1, &m_buffColourAttachment);
	glBindTexture(GL_TEXTURE_2D, m_buffColourAttachment);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	m_parentRenderer->checkErrors();

	// Generate FBOs
	glGenFramebuffers(1, &m_sceneFBO);
	glGenFramebuffers(1, &m_processFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);

	// Attaching attachments to sceneFBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_buffDepthAttachment, 0);		// Depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_buffDepthAttachment, 0);		// Stencil attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_buffColourAttachment, 0);		// Colour attackment (only one?)
	m_parentRenderer->checkErrors();


	// Checking if FBO attachment was successful
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !m_buffDepthAttachment || !m_buffColourAttachment) {
		std::cout << "FBO Attachment failed " << std::endl;
		m_parentRenderer->checkErrors();
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);

}

void PostProcessor::drawSceneToFBO()
{


	//m_sceneFBO, m_sceneShader
	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);

	m_parentRenderer->clearBuffers();
	m_parentRenderer->setCurrentShader(m_sceneShader);

	m_parentRenderer->changeProjection(Orthographic);
	m_parentRenderer->updateShaderMatrices();
	m_parentRenderer->checkErrors();


	m_parentRenderer->drawAllRenderObjects();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void PostProcessor::drawPostProcess()
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

	m_parentRenderer->setCurrentShader(m_processShader);	// Change to our post processing shader
	m_parentRenderer->changeProjection(Orthographic);
				
	m_parentRenderer->setViewMatrix(Matrix4());				// set to identitiy matrix
	m_parentRenderer->updateShaderMatrices();
	glDisable(GL_DEPTH_TEST);

	// Update screen size uniform.
	Shader* currentShader = m_parentRenderer->getCurrentShader();
	GLuint screenSizeID = glGetUniformLocation(currentShader->getProgram(), "screenSize");
	glUniform2f(screenSizeID, (width), (height));

	GLuint timeID = glGetUniformLocation(currentShader->getProgram(), "time");
	glUniform1f(timeID, (float)(glfwGetTime()*10.0f));

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_buffColourAttachment, 0);
	m_screenQuad->setTexture(m_buffColourAttachment);
	m_screenQuad->draw();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}

void PostProcessor::presentScene()
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_parentRenderer->setCurrentShader(m_sceneShader);
	m_parentRenderer->changeProjection(Orthographic);
	m_parentRenderer->setViewMatrix(Matrix4());
	m_parentRenderer->updateShaderMatrices();
	m_parentRenderer->checkErrors();

	m_screenQuad->setTexture(m_buffColourAttachment);
	m_screenQuad->draw();
	glUseProgram(0);

}
