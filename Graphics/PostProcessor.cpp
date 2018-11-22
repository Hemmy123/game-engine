#include "PostProcessor.h"

#include "FilePaths.h"

PostProcessor::PostProcessor(Renderer * r, Mesh* screenQuad) :
	m_screenQuad(screenQuad),
	m_parentRenderer(r)
{
	createShaders();
	generateProcessFBO();
	attachAttachments();
}

PostProcessor::~PostProcessor()
{

	glDeleteTextures(2, m_processColourAttachment);
	glDeleteTextures(1, &m_processDepthAttachment);

	glDeleteFramebuffers(1, &m_processFBO);
}



void PostProcessor::generateProcessFBO()
{
	glGenFramebuffers(1, &m_processFBO);

	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();


	// Create colour attachemnts
	for (int i = 0; i < 2; i++) {
		glGenTextures(1, &m_processColourAttachment[i]);
		glBindTexture(GL_TEXTURE_2D, m_processColourAttachment[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	
	// Create depth attachment
	glGenTextures(1, &m_processDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_processDepthAttachment);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

}

void PostProcessor::attachAttachments()
{



}

void PostProcessor::drawPostProcessing(GLuint colourAttachment, Effect effect)
{

	Shader* shader = m_effectsMap.at(effect);

	switch (effect) {

	case(GaussianBlur): {
		drawPostProcessPingPong(colourAttachment, shader, 10);
		break;
	} 

	case(Bloom): {
		Shader* filterShader = m_effectsMap.at(BrightnessFilter);
		drawPostProcess(colourAttachment, filterShader);

		Shader* blurShader = m_effectsMap.at(GaussianBlur);
		drawPostProcessPingPong(m_processColourAttachment[0], blurShader, 10);

		drawPostProcess(colourAttachment, m_combineShader);

		//combine on colourattachment 0
		//drawPostProcessPingPong(m_processColourAttachment[0], shader, 1);
		break;
	}
	case(SineBlur): {
		drawPostProcess(colourAttachment, shader);
		break;
	}
	case(BrightnessFilter): {
		
		Shader* brightnessFilter = m_effectsMap.at(BrightnessFilter);
		drawPostProcess(colourAttachment, shader);
		break;
	}
	default :break;

	}

	drawFinalPostProcess(colourAttachment);


}

void PostProcessor::drawFinalPostProcess(GLuint colourAttachment)
{
	// Bind the process FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_processFBO);
	// assumes the final effect is on m_processColourAttachment[0]
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);
	glUseProgram(m_passThroughShader->getProgram());

	m_screenQuad->setTexture(m_processColourAttachment[0]);
	m_screenQuad->setTextureType(Texture_2D);
	m_screenQuad->bindTexture();
	m_parentRenderer->checkErrors();

	m_screenQuad->draw();
	m_parentRenderer->checkErrors();



	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void PostProcessor::drawPostProcessPingPong(GLuint colourAttachment, Shader* shader, int passes)
{
	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();

	// Bind the process FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_processFBO);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// Attach a texture image to the current frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_processColourAttachment[1], 0);


	m_parentRenderer->setCurrentShader(shader);	// Change to our post processing shader
	m_parentRenderer->changeProjection(Projection::Orthographic);
	m_parentRenderer->setViewMatrix(Matrix4());				// set to identitiy matrix
	m_parentRenderer->updateShaderMatrices();

	glDisable(GL_DEPTH_TEST);



	// Update screen size uniform.
	Shader* currentShader = m_parentRenderer->getCurrentShader();
	GLuint screenSizeLocation = glGetUniformLocation(currentShader->getProgram(), "pixelSize");
	glUniform2f(screenSizeLocation, (1.0/width), (1.0/height));

	m_parentRenderer->checkErrors();


	GLuint timeID = glGetUniformLocation(currentShader->getProgram(), "time");
	glUniform1f(timeID, (float)(glfwGetTime()*10.0f));
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffColourAttachment, 0);
	m_parentRenderer->checkErrors();

	for (int i = 0; i < passes; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_processColourAttachment[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->getProgram(), "isVertical"), 0); // set vertical to false
		m_parentRenderer->checkErrors();

		// if it's the first pass, use the scene colour attachment
		if (i == 0) {
			m_screenQuad->setTexture(colourAttachment);
		}
		else {
			m_screenQuad->setTexture(m_processColourAttachment[0]);
		}

		m_screenQuad->setTextureType(Texture_2D);
		m_screenQuad->bindTexture();
		m_screenQuad->draw();


		//Now to swap  the  colour  buffers , and do the  second  blur  pass
		glUniform1i(glGetUniformLocation(currentShader->getProgram(),
			"isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_processColourAttachment[0], 0);

		m_screenQuad->setTexture(m_processColourAttachment[1]);
		m_screenQuad->setTextureType(Texture_2D);
		m_screenQuad->bindTexture();
		m_screenQuad->draw();
	}


	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void PostProcessor::drawPostProcess(GLuint colourAttachment, Shader * shader)
{
	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();

	// Bind the process FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_processFBO);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_processColourAttachment[0], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	m_parentRenderer->setCurrentShader(shader);	// Change to our post processing shader
	m_parentRenderer->changeProjection(Projection::Orthographic);
	m_parentRenderer->setViewMatrix(Matrix4());				// set to identitiy matrix
	m_parentRenderer->updateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	// Update screen size uniform.
	Shader* currentShader = m_parentRenderer->getCurrentShader();
	GLuint screenSizeLocation = glGetUniformLocation(currentShader->getProgram(), "pixelSize");
	glUniform2f(screenSizeLocation, (1.0 / width), (1.0 / height));

	GLuint timeID = glGetUniformLocation(currentShader->getProgram(), "time");
	glUniform1f(timeID, (float)(glfwGetTime()*10.0f));


	m_screenQuad->setTexture(colourAttachment);
	m_screenQuad->setTextureType(Texture_2D);
	m_screenQuad->bindTexture();
	m_screenQuad->draw();

	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void PostProcessor::createShaders()
{
	Shader* gaussianBlurShader	=  new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"GaussianBlur_Frag.glsl");
	Shader* bloomShader			=  new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Bloom_Frag.glsl");
	Shader* sineBlur			= new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Process_Frag.glsl");
	Shader* brightnessFilter	= new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"BrightFilter_Frag.glsl");

	m_combineShader			= new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"CombineTex_Frag.glsl");
	m_passThroughShader		= new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Textured_Frag.glsl");

	m_effectsMap.insert({ GaussianBlur, gaussianBlurShader });
	m_effectsMap.insert({ Bloom, bloomShader });
	m_effectsMap.insert({ SineBlur, sineBlur });
	m_effectsMap.insert({ BrightnessFilter, brightnessFilter });



}


