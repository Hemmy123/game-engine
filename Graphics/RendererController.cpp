#include "RendererController.h"
#include "FilePaths.h"

RendererController::RendererController(int height, int width, SceneManager* scene):
	m_height(height), m_width(width)
{

	m_renderer = new Renderer(height, width, scene);

	m_sceneShader = new Shader(SHADERVERTDIR"PassThrough_Vert.glsl", SHADERFRAGDIR"Scene_Frag.glsl");
	generateFBO();

	m_screenQuad = Mesh::generateQuad();
	m_screenQuad->bufferData();

	m_postProcessor = new PostProcessor(m_renderer, m_screenQuad);
	m_skybox		= new Skybox(m_renderer, m_screenQuad);
	m_anaglyph3D = new Anaglyph3D(m_renderer);

}


RendererController::~RendererController()
{

	glGenFramebuffers(1, &m_sceneFBO);
	glDeleteTextures(1, &m_buffColourAttachment);
	glDeleteTextures(1, &m_buffDepthAttachment);
	
	delete m_screenQuad;
	delete m_sceneShader;

	delete m_renderer;
	delete m_skybox;
	delete m_postProcessor;
}

void RendererController::init()
{
}

void RendererController::update(float msec)
{
	glfwPollEvents();
	
	m_renderer->updateScene(msec);

	

	if (m_settings.skybox) {
		m_skybox->drawSkybox(m_screenQuad, m_sceneFBO);
	}
	
	// if 3D effect, skip this step?

	if (m_settings.anaglyph3D) {
		m_anaglyph3D->render(m_screenQuad, m_sceneFBO, m_buffColourAttachment);
	}
	else {
		m_renderer->renderScene(m_screenQuad, m_sceneShader, m_sceneFBO);
	}


	if (m_settings.postProcessing) {
		m_postProcessor->drawPostProcess(m_buffColourAttachment);
	}


	m_renderer->presentScene(m_screenQuad, m_sceneShader, m_buffColourAttachment);
	//m_renderer->clearBuffers();
	m_renderer->swapBuffers();
}


// ----- Pass through renderer methods ----- //

void RendererController::generateFBO()
{

	glGenTextures(1, &m_buffDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_buffDepthAttachment);
	m_renderer->checkErrors();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	m_renderer->checkErrors();

	// Note:
	// GL_DEPTH24_STENCIL8 and GL_DEPTH_STENCIL because it's a depth texture

	glGenTextures(1, &m_buffColourAttachment);
	glBindTexture(GL_TEXTURE_2D, m_buffColourAttachment);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	m_renderer->checkErrors();

	// Generate FBOs
	glGenFramebuffers(1, &m_sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);

	// Attaching attachments to sceneFBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_buffDepthAttachment, 0);		// Depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_buffDepthAttachment, 0);		// Stencil attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_buffColourAttachment, 0);		// Colour attachment
	m_renderer->checkErrors();


	// Checking if FBO attachment was successful
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !m_buffDepthAttachment || !m_buffColourAttachment) {
		std::cout << "FBO Attachment failed " << std::endl;
		m_renderer->checkErrors();
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

bool RendererController::checkWindow() {
	return m_renderer->checkWindow();
}


Camera* RendererController::getCamera() {
	return m_renderer->getCamera();
}

void RendererController::setShaderLight(Shader* s, Light* l) {
	m_renderer->setShaderLight(s, *l);
}
