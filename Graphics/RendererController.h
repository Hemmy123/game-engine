#pragma once
#include "Renderer.h"
#include "Skybox.h"
#include "PostProcessor.h"



// A simple struct to toggle features on and off
struct RendererSettings{
	bool skybox;
	bool postProcessing;


};

class RendererController
{
public:
	RendererController();
	~RendererController();

	void init();
	void update();
 


private:

	RendererSettings m_settings;
	Renderer*		 m_renderer;
	Skybox*			 m_skybox;
	PostProcessor*	 m_postProcessor;
};

