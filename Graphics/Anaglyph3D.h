#pragma once

#include "Renderer.h"

class Anaglyph3D
{
public:
	Anaglyph3D(Renderer* r);
	~Anaglyph3D();


	void createFBOs();

	void render(Mesh* quad, GLuint fbo, GLuint colourAttachment);
	void renderRight();
	void renderLeft();

	void createLeftFBO();
	void createRightFBO();
	
private:


	int m_width, m_height;
	Renderer* m_parentRenderer;
	Shader* m_defaultShader;
	Shader* m_3DShader;

	// ----- Left ----- //
	GLuint m_leftFBO;
	GLuint m_leftColourAttachment;
	GLuint m_leftDepthAttachment;

	// ----- Right ----- //
	GLuint m_rightFBO;
	GLuint m_rightColourAttachment;
	GLuint m_rightDepthAttachment;
};

