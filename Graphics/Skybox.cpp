////========================================
// Class: Skybox
// Author: Hemmy
// Date: 29/07/2018
// Description:
//
// ========================================

#include "Skybox.h"
#include "FilePaths.h"
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL2.h>

Skybox::Skybox(Renderer* r, Mesh* screenQuad):
	m_quad(screenQuad),
	m_parentRenderer(r)
{
	m_skyboxShader = new Shader(SHADERVERTDIR"Skybox_Vert.glsl", SHADERFRAGDIR"Skybox_Frag.glsl");


	m_cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"Skybox/posx.jpg",
		TEXTUREDIR"Skybox/negx.jpg",
		TEXTUREDIR"Skybox/posy.jpg",
		TEXTUREDIR"Skybox/negy.jpg",
		TEXTUREDIR"Skybox/posz.jpg",
		TEXTUREDIR"Skybox/negz.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (m_cubeMap == 0) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());

	}
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);



}

Skybox::~Skybox()
{
}


void Skybox::drawSkybox() {
	glDepthMask(GL_FALSE);

	m_parentRenderer->setCurrentShader(m_skyboxShader);
	m_parentRenderer->updateShaderMatrices();

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

	glUniform1i(glGetUniformLocation(m_skyboxShader->getProgram(), "cubeTex"), 2);
	m_quad->setTexture(m_cubeMap);
	m_quad->draw();

	m_parentRenderer->checkErrors();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}
