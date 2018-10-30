#pragma once
#include "Renderer.h"
#include "Skybox.h"
#include "PostProcessor.h"

class RendererController
{
public:
	RendererController();
	~RendererController();

	void init();


private:
	Renderer*		m_renderer;
	Skybox*			m_skybox;
	PostProcessor*	m_postProcessor;
};

