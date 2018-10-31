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

class Skybox{
public:
	Skybox(Renderer* r, Mesh* screenQuad);
	~Skybox();
	
	void drawSkybox();


private:
	
	Renderer* m_parentRenderer;
	Shader* m_skyboxShader;

	Mesh* m_quad;

	// -- Misc
	//Mesh*	m_waterQuad;
	GLuint 	m_cubeMap;
	float 	m_waterRotate;
};


