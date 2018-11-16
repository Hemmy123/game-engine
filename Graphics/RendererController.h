#pragma once
#include "Renderer.h"
#include "Skybox.h"
#include "PostProcessor.h"
#include "Anaglyph3D.h"
#include "Shadows.h"
#include "Level.h"
#include "DeferredRenderer.h"

class RendererController
{
public:
	RendererController(int height, int width, SceneManager* scene);
	~RendererController();

	void init();
	void update(float msec);
	void initCamera();

	void setSettings();

	void createCamera(InterfaceHandler* ih) { 
		m_renderer->createCamera(ih); 
		initCamera();
	}

	GLFWwindow* getWindow() const { return m_renderer->getWindow(); }

	void updateLighting();

	void setSetting(RendererSettings setting) { m_settings = setting; }


	// -- Rendering to quad -- //

	void generateFBO();

	// ----- Pass through renderer methods ----- //

	bool checkWindow();
	Camera* getCamera();
	void setShaderLight(Shader* s, Light* l);

private:

	int m_height, m_width;

	// Remember to delete these in the right order
	RendererSettings m_settings;

	// ----- screen quads ----- //
	Mesh*		m_screenQuad;
	Shader*		m_sceneShader;

	GLuint		m_sceneFBO;
	GLuint		m_buffColourAttachment;
	GLuint		m_buffDepthAttachment;


	// ----- Renderer classes ----- //
	Renderer*			m_renderer;

	SceneManager*		m_sceneManager;
	Skybox*				m_skybox;
	PostProcessor*		m_postProcessor;
	Anaglyph3D*			m_anaglyph3D;
	Shadows*			m_shadows;
	DeferredRenderer*	m_deferred;
};

