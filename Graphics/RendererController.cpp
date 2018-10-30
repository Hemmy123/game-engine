#include "RendererController.h"



RendererController::RendererController()
{
	m_renderer = new Renderer(800, 1024);

	m_settings.skybox			= true;
	m_settings.postProcessing	= true;
}


RendererController::~RendererController()
{
}

void RendererController::init()
{
}

void RendererController::update()
{
	if (m_settings.skybox) {
		m_skybox->drawSkybox();
	} 
	if (m_settings.postProcessing) {
		m_postProcessor->drawSceneToFBO();
		m_postProcessor->drawPostProcess();
		m_postProcessor->presentScene();
	}
	// renderer->renderScene?
}
