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
	m_refectShader = new Shader(SHADERVERTDIR"PerPixel_Vert.glsl", SHADERFRAGDIR"Reflect_Frag.glsl");

	m_cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"Skyboxes/2/right.jpg",
		TEXTUREDIR"Skyboxes/2/left.jpg",
		TEXTUREDIR"Skyboxes/2/top.jpg",
		TEXTUREDIR"Skyboxes/2/bottom.jpg",
		TEXTUREDIR"Skyboxes/2/front.jpg",
		TEXTUREDIR"Skyboxes/2/back.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);




	if (m_cubeMap == 0) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());

	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);



}

Skybox::~Skybox()
{

	delete m_skyboxShader;
	delete m_refectShader;
}


void Skybox::drawSkybox(Mesh* quad, GLuint fbo) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	m_parentRenderer->clearBuffers();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); 
	glDepthMask(GL_FALSE);

	m_parentRenderer->setCurrentShader(m_skyboxShader);

	m_parentRenderer->updateShaderMatrices();

	GLuint texCubeLocation = glGetUniformLocation(m_skyboxShader->getProgram(), "cubeTex");

	glUniform1i(texCubeLocation, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);


	quad->setTexture(m_cubeMap);
	quad->setTextureType(Cube_Map);
	quad->bindTexture();
	quad->draw();

	m_parentRenderer->checkErrors();

	
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Skybox::drawRefection(Mesh* quad, GLuint fbo,HeightMap * heightmap, Vector3 cameraPos)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	m_parentRenderer->setCurrentShader(m_refectShader);
	m_parentRenderer->updateShaderMatrices();

	GLuint cameraPosLoc		= glGetUniformLocation(m_refectShader->getProgram(), "cameraPos");
	GLuint diffuseTexLoc	= glGetUniformLocation(m_refectShader->getProgram(), "diffuseTex");
	GLuint cubeTexLoc		= glGetUniformLocation(m_refectShader->getProgram(), "cubeTex");
	

	glUniform3fv(cameraPosLoc, 1, (float*)&cameraPos);
	glUniform1i(diffuseTexLoc, 0);
	glUniform1i(cubeTexLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

	m_parentRenderer->setTextureMatrix(Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(2, Vector3(0.0f, 0.0f, 1.0f)));
	m_parentRenderer->updateShaderMatrices();

	heightmap->draw();

	glUseProgram(0);

}
