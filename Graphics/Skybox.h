////========================================
// Class: Skybox
// Author: Hemmy
// Date: 29/07/2018
// Description:
//
// ========================================
#pragma once

#include "Shader.h"
#include <stdio.h>
#include "Renderer.h"
#include "PerlinShaderInterface.h"
class Skybox{
public:
	Skybox(Renderer* r, Mesh* screenQuad);
	~Skybox();
	
	void drawSkybox(Mesh* quad, GLuint fbo);

	void drawRefection(Mesh* quad, GLuint fbo, RenderObject* heightmap, Vector3 cameraPos);


private:
	
	Renderer*	m_parentRenderer;
	Shader*		m_skyboxShader;
	Shader*		m_refectShader;
	Shader*		m_perlinReflectShader;
	PerlinShaderInterface* m_perlinShaderInterface;

	Mesh* m_quad;

	// -- Misc
	//Mesh*	m_waterQuad;
	Light*	m_light;
	GLuint 	m_cubeMap;
	GLuint 	m_waterTex;
	float 	m_waterRotate;
};


