#include "Shadows.h"
#include "FilePaths.h"

Shadows::Shadows(Renderer * parentRenderer):
	m_parentRenderer(parentRenderer)
{
	
	m_sceneShader = new Shader(
		SHADERVERTDIR"shadowScene_vert.glsl",
		SHADERFRAGDIR"shadowScene_frag.glsl");

	m_shadowShader = new Shader(
		SHADERVERTDIR"shadow_vert.glsl",
		SHADERFRAGDIR"shadow_frag.glsl");


	Vector3 pos(100, 700, -400);
	Vector4 col(1, 1, 1, 1);
	float radius	= 6000.0f;
	m_light			= new Light(pos, col, radius);

	static const float biasValues[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};
	m_biasMatrix = Matrix4(const_cast<float*>(biasValues));
	generateFBO();
}	

Shadows::~Shadows()
{
	glDeleteTextures(1, &m_shadowTex);
	glDeleteFramebuffers(1, &m_shadowFBO);

	delete m_light;
	delete m_shadowShader;
	delete m_sceneShader;
}

void Shadows::drawShadowScene(){
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
	glEnable(GL_DEPTH_TEST);

	// Clear depth buffer since we are about to draw to it.
	glClear(GL_DEPTH_BUFFER_BIT);

	// Sets the view port to be the origin?
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	// Don't draw any colours! (R,G,B,A)
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	m_parentRenderer->changeProjection(Perspective);
	m_parentRenderer->setCurrentShader(m_shadowShader);

	Matrix4 lightViewMatrix = Matrix4::BuildViewMatrix(
		m_light->getPosition(), Vector3(0, 0, 0));

	m_parentRenderer->setViewMatrix(lightViewMatrix);
	m_parentRenderer->setTextureMatrix(m_lightTextureMatrix);

	m_lightTextureMatrix = m_biasMatrix *
		(m_parentRenderer->getProjectionMatrix() * m_parentRenderer->getViewMatrix());


	m_parentRenderer->updateShaderMatrices();
	glUniformMatrix4fv(glGetUniformLocation(m_shadowShader->getProgram(), "textureMatrix"),	1, false, (float*)&m_lightTextureMatrix);
	m_parentRenderer->checkErrors();

	m_parentRenderer->drawAllMeshes();
	m_parentRenderer->checkErrors();


	// back to default settings
	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, m_parentRenderer->getWidth(), m_parentRenderer->getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void Shadows::drawCombinedScene(GLuint sceneFbo){

	glBindFramebuffer(GL_FRAMEBUFFER, sceneFbo);

	m_parentRenderer->setCurrentShader(m_sceneShader);
	//m_parentRenderer->updateShaderMatrices();
	glUseProgram(m_sceneShader->getProgram());
	GLuint diffuseTexLoc	= glGetUniformLocation(m_sceneShader->getProgram(), "diffuseTex");
	GLuint bumpTexLoc		= glGetUniformLocation(m_sceneShader->getProgram(), "bumpTex");
	GLuint shadowTexLoc		= glGetUniformLocation(m_sceneShader->getProgram(), "shadowTex");
	GLuint cameraLoc		= glGetUniformLocation(m_sceneShader->getProgram(), "cameraPos");
	

	glUniform1i(diffuseTexLoc, 0);
	m_parentRenderer->checkErrors();

	glUniform1i(bumpTexLoc, 1);
	m_parentRenderer->checkErrors();

	glUniform1i(shadowTexLoc, 2);
	m_parentRenderer->checkErrors();

	Vector3 cameraPos = m_parentRenderer->getCamera()->GetPosition();
	glUniform3fv(cameraLoc, 1, (float*)&cameraPos);
	m_parentRenderer->checkErrors();


	m_parentRenderer->setShaderLight(m_sceneShader, *m_light);
	m_parentRenderer->checkErrors();


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_shadowTex);

	Matrix4 viewMatrix = m_parentRenderer->getCamera()->BuildViewMatrix();
	m_parentRenderer->setViewMatrix(viewMatrix);

	m_parentRenderer->updateShaderMatrices();

	// Has to use somesort of raw draw method
	//m_parentRenderer->drawAllRenderObjects();
	//glUniformMatrix4fv(glGetUniformLocation(m_shadowShader->getProgram(), "textureMatrix"), 1, false, (float*)&m_lightTextureMatrix);
	m_parentRenderer->drawAllMeshes();
	
	
	m_parentRenderer->checkErrors();

	glUseProgram(0);

}

void Shadows::drawScene(GLuint sceneFBO)
{
	// clear buffers?
	if (count > 1000) {
		count = 0;
	}
	count += 0.2;

	m_light->setPosition(Vector3(100, count, -400));

	drawShadowScene();
	drawCombinedScene(sceneFBO);
}

int Shadows::generateFBO()
{

	// ===== Generate Texture ===== //

	glGenTextures(1, &m_shadowTex);
	glBindTexture(GL_TEXTURE_2D, m_shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	

	// ===== Generate FBO ===== //
	glGenFramebuffers(1, &m_shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
	// Creating depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowTex, 0);
	// Which colour buffer to draw to when draw is called.
	// GL_NONE because we only want the depth texture.
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}
