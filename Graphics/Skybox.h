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
class Skybox{
public:
	Skybox(Renderer);
	~Skybox();
	
	void drawSkybox();


private:
	
	Shader* m_skyboxShader;

	// -- Misc
	//Mesh*	m_waterQuad;
	GLuint 	m_cubeMap;
	float 	m_waterRotate;
};


 /* Skybox_hpp */
