#include "RendererController.h"



RendererController::RendererController(int height, int width ):
	m_height(height), m_width(width)
{
	m_renderer		= new Renderer(height, width);
	m_postProcessor = new PostProcessor(m_renderer);
	m_skybox		= new Skybox(m_renderer);

}


RendererController::~RendererController()
{
}

void RendererController::init()
{
}

void RendererController::update(float msec)
{
	glfwPollEvents();
	m_renderer->updateScene(msec);
	m_renderer->clearBuffers();

	if (m_settings.skybox) {
		m_skybox->drawSkybox();
	} 
	if (m_settings.postProcessing) {
		m_postProcessor->drawSceneToFBO();
		m_postProcessor->drawPostProcess();
		m_postProcessor->presentScene();
	}

	if (!m_settings.skybox && !m_settings.postProcessing) {
		m_renderer->renderScene();

	}
	m_renderer->swapBuffers();
}


// ----- Pass through renderer methods ----- //

bool RendererController::checkWindow() {
	return m_renderer->checkWindow();
}

vector<RenderObject*> RendererController::getOpaqueObjects() const{
	return m_renderer->getOpaqueObjects();
}

Camera* RendererController::getCamera() {
	return m_renderer->getCamera();
}

void RendererController::setShaderLight(Shader* s, Light* l) {
	m_renderer->setShaderLight(s, *l);
}

void RendererController::setRenderObjects(vector<RenderObject*> renderObjects) {
	m_renderer->setRenderObjects(renderObjects);
}
