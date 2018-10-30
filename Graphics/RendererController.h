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
	RendererController(int height, int width);
	~RendererController();

	void init();
	void update(float msec);
 
	void createCamera(InterfaceHandler* ih) { m_renderer->createCamera(ih); }

	GLFWwindow* getWindow() const { return m_renderer->getWindow(); }

	

	void setSetting(RendererSettings setting) { m_settings = setting; }


	// ----- Pass through renderer methods ----- //

	bool checkWindow();
	vector<RenderObject*> getOpaqueObjects() const;
	Camera* getCamera();
	void setShaderLight(Shader* s, Light* l);

	void setRenderObjects(vector<RenderObject*> renderObjects);

private:

	int m_height, m_width;


	// Remember to delete these in the right order
	RendererSettings m_settings;
	Renderer*		 m_renderer;
	Skybox*			 m_skybox;
	PostProcessor*	 m_postProcessor;
};

