//
//  Renderer.cpp
//  Graphics
//
//  Created by Hemmy on 30/05/2018.
//  Copyright © 2018 Hemmy. All rights reserved.
//

#include "Renderer.h"
#include "FilePaths.h"

//Renderer::Renderer(): WIDTH(900),HEIGHT(700)
Renderer::Renderer(int height, int width, SceneManager* sm) :
	MasterRenderer(height, width),
	sceneManager(sm),
	m_clearColour(Vector4(0.3, 0.5, 0.4, 1)) 
{
	if (init() != 0) {
		std::cout << "OpenGL Failed to initialize!" << std::endl;
	};


	m_aspectRatio = (float)m_actualWidth / (float)m_actualHeight;
	//m_ortho = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	m_ortho = Matrix4::Orthographic(1, -1, 1, -1, 1, -1);
	m_persp = Matrix4::Perspective(1, m_viewDistance, m_aspectRatio, m_fov);
	defaultGLSettings();

}

Renderer::~Renderer() {

	// Note: Renderobjects should be delete by whatever is 
	// managing them, not the renderer!

	glfwTerminate();
	delete m_camera;
}

void Renderer::defaultGLSettings()
{
	// Cull faces we can't see
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Depth test so stuff doesn't render on top of each other;
	glEnable(GL_DEPTH_TEST);

	// Blend func for transparent objects;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	glClearColor(
		m_clearColour.x,
		m_clearColour.y,
		m_clearColour.z,
		m_clearColour.w);
	checkErrors();

}





void Renderer::update(float msec) {
	m_dt = msec;
	checkErrors();
	updateScene(m_dt);

	clearBuffers();
	//renderScene();
	swapBuffers();
}

void Renderer::createCamera(InterfaceHandler *ih) {
	m_camera = new Camera(ih);
}



void Renderer::renderScene(Mesh* quad, Shader* shader, GLuint fbo) {


	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	setCurrentShader(shader);
	changeProjection(Perspective);
	updateShaderMatrices();
	drawAllRenderObjects();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::drawRenderObject(const RenderObject &o) {
	m_modelMatrix = o.getModelMatrix();
	if (o.getShader() && o.getMesh()) {
		GLuint program = o.getShader()->getProgram();
		setCurrentShader(o.getShader());
		glUseProgram(program);

		if (o.getMesh()->hasBumpTexture()) {
			GLuint diffuseLoc = glGetUniformLocation(program, "diffuseTex");
			GLuint bumpTexLoc = glGetUniformLocation(program, "bumpTex");
			glUniform1i(diffuseLoc, 0);
			glUniform1i(bumpTexLoc, 1);
		}

		updateShaderMatrices();
		//o.getMesh()->setTextureType(Texture_2D);
		o.getMesh()->bindTexture();

		o.draw();
	}

	for (auto iter : o.getChildren()) {
		
		drawRenderObject(*iter);
	}

}

void Renderer::drawMesh(const RenderObject & o)
{
	Mesh* objMesh	= o.getMesh();

	if (objMesh) {
		//m_modelMatrix.ToIdentity();

		// For mapping shadows
		Matrix4 modelMatrix = o.getModelMatrix();

		Matrix4 tempMatrix = m_textureMatrix * modelMatrix;
		GLuint texMatLoc = glGetUniformLocation(m_currentShader->getProgram(),"textureMatrix");
		GLuint modelMatLoc = glGetUniformLocation(m_currentShader->getProgram(),"modelMatrix");
		glUniformMatrix4fv(texMatLoc, 1, false, *&tempMatrix.values);
		glUniformMatrix4fv(modelMatLoc, 1, false, *&modelMatrix.values);
		
		objMesh->bindTexture();
		objMesh->draw();
	}

	for (auto iter : o.getChildren()) {

		drawMesh(*iter);
	}
}

void Renderer::drawAllRenderObjects(){
	for (auto iter : sceneManager->getOpaque() ) {
		drawRenderObject(*iter); 
	}
	for (auto iter : sceneManager->getTransparent()) {
		drawRenderObject(*iter); 
	}
}

void Renderer::drawAllMeshes()
{

	for (auto iter : sceneManager->getOpaque()) {
		drawMesh(*iter);
	}
	for (auto iter : sceneManager->getTransparent()) {
		drawMesh(*iter);
	}
}

void Renderer::changeProjection(Projection proj)
{
	switch (proj) {
	case Orthographic: m_projMatrix = m_ortho; break;
	case Perspective: m_projMatrix = m_persp; break;
	}

}


void Renderer::updateScene(float msec) {

	if (m_camera == nullptr) {
		std::cout << "No camera has been created/set" << std::endl;
	}
	else {
		m_camera->update(msec);
		m_viewMatrix = m_camera->BuildViewMatrix();
		updateRenderObjects(msec);
	}


}

void Renderer::updateRenderObjects(float msec) {

	for (auto iter : sceneManager->getOpaque()) {
		iter->update(msec);
	}

	for (auto iter : sceneManager->getTransparent()) {
		iter->update(msec);
	}


}

void Renderer::updateShaderMatrices( ) {

	GLuint program = m_currentShader->getProgram();
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"),	1, false, (float*)&m_modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"),		1, false, (float*)&m_viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"),		1, false, (float*)&m_projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "textureMatrix"),	1, false, (float*)&m_textureMatrix);

	Matrix4 mvp = m_projMatrix * m_viewMatrix * m_modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, false, (float*)&mvp);




}

void Renderer::presentScene(Mesh* quad, Shader* sceneShader, GLuint texture) {

	// Default framebuffer to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	clearBuffers();	// Clears defualt frame buffer, not the one objects are rendererd to!
	setCurrentShader(sceneShader);
	changeProjection(Orthographic);
	setViewMatrix(Matrix4());
	updateShaderMatrices();

	quad->setTexture(texture);
	quad->setTextureType(Texture_2D);
	quad->bindTexture();
	quad->draw();

	glUseProgram(0);
	
}


void Renderer::setShaderLight(Shader* shader, Light &light) {

	GLuint program = shader->getProgram();
	glUseProgram(program);


	GLint posLoc = glGetUniformLocation(program, "lightPos");
	GLint colLoc = glGetUniformLocation(program, "lightColour");
	GLint radLoc = glGetUniformLocation(program, "lightRadius");
	Vector3 pos = light.getPosition();
	Vector4 col = light.getColour();
	float 	rad = light.getRadius();

	glUniform3fv(posLoc, 1, (float*)&pos);
	glUniform4fv(colLoc, 1, (float*)&col);
	glUniform1f(radLoc, rad);
	checkErrors();



}


















