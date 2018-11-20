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
	m_waterTex = SOIL_load_OGL_texture(SHADERVERTDIR"water.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	m_cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"Skyboxes/2/right.jpg",
		TEXTUREDIR"Skyboxes/2/left.jpg",
		TEXTUREDIR"Skyboxes/2/top.jpg",
		TEXTUREDIR"Skyboxes/2/bottom.jpg",
		TEXTUREDIR"Skyboxes/2/front.jpg",
		TEXTUREDIR"Skyboxes/2/back.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	Vector3 pos(100.0f, 400.0f, 500.0f);
	Vector4 col(1.0f, 1.0f, 1.0f, 1.0f);
	float radius = 6000;

	m_light = new Light(pos,col,radius);


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

	glUniform1i(texCubeLocation, TextureUniforms::CubeMap);
	
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::CubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);


	quad->setTexture(m_cubeMap);
	quad->setTextureType(Cube_Map);
	quad->bindTexture();
	quad->draw();

	m_parentRenderer->checkErrors();

	
	// Back to default settings
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Skybox::drawRefection(Mesh* quad, GLuint fbo, RenderObject * obj, Vector3 cameraPos)
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	HeightMap* heightmap = static_cast<HeightMap*>(obj->getMesh());

	m_parentRenderer->setCurrentShader(m_refectShader);
	m_parentRenderer->setShaderLight(m_refectShader, *m_light);
	m_parentRenderer->setModelMatrix(obj->getModelMatrix());
	m_parentRenderer->setTextureMatrix(Matrix4::Scale(Vector3(1,1,1)));

	m_parentRenderer->updateShaderMatrices();

	GLuint cameraPosLoc		= glGetUniformLocation(m_refectShader->getProgram(), "cameraPos");
	GLuint diffuseTexLoc	= glGetUniformLocation(m_refectShader->getProgram(), "diffuseTex");
	GLuint cubeTexLoc		= glGetUniformLocation(m_refectShader->getProgram(), "cubeTex");
	
	// Set camera pos uniform
	glUniform3fv(cameraPosLoc, 1, (float*)&cameraPos);

	// set diffuse tex uniform
	GLuint waterTex = heightmap->getTexture();
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::Diffuse);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(diffuseTexLoc, TextureUniforms::Diffuse);
	m_parentRenderer->checkErrors();

	// set cubmap tex uniform
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::CubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
	glUniform1i(cubeTexLoc, TextureUniforms::CubeMap);
	m_parentRenderer->checkErrors();

	m_parentRenderer->updateShaderMatrices();

	heightmap->draw();
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::Default);

	glUseProgram(0);

}
