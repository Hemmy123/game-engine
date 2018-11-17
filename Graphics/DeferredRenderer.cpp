#include "DeferredRenderer.h"
#include "FilePaths.h"

DeferredRenderer::DeferredRenderer(Renderer * parentRenderer,Mesh* quad):
	m_quad(quad),
	m_parentRenderer(parentRenderer)
{
	m_sceneShader = new Shader(SHADERVERTDIR"Bump_Vert.glsl", SHADERFRAGDIR"Buffer_Frag.glsl");
	m_sceneShader = new Shader(SHADERVERTDIR"Combine_Vert.glsl", SHADERFRAGDIR"Combine_Frag.glsl");
	m_sceneShader = new Shader(SHADERVERTDIR"PointLight_Vert.glsl", SHADERFRAGDIR"PointLight_Frag.glsl");

	initBuffers();
}


DeferredRenderer::~DeferredRenderer()
{
	glDeleteFramebuffers(1,&m_Gbuffer);
	glDeleteTextures(1,&m_GColour);
	glDeleteTextures(1,&m_GDepth);
	glDeleteTextures(1,&m_GNormal);

	glDeleteFramebuffers(1,&m_lightFBO);
	glDeleteTextures(1,&m_lightEmissive);
	glDeleteTextures(1,&m_lightSpecular);




}

void DeferredRenderer::initBuffers()
{
	generateBuffers();
	generateAllTextures();
	attachTextures();
}

void DeferredRenderer::createLights()
{
	m_lights = new Light[rowLenth * rowLenth];

	Mesh* lightMesh = Mesh::readObjFile("");

	int RAW_WIDTH = 257;
	int RAW_HEIGHT = 257;
	int HEIGHTMAP_X = 16;
	int HEIGHTMAP_Z = 16;

	for (int x = 0; x < rowLenth; ++x) {	
		for (int z = 0; x < rowLenth; ++z) {
			Light &l = m_lights[(x * rowLenth) + z];

			 float xPos = (RAW_WIDTH * HEIGHTMAP_X / (rowLenth - 1)) * x;
			 float zPos = (RAW_HEIGHT * HEIGHTMAP_Z / (rowLenth - 1)) * z;
			 l.setPosition(Vector3(xPos, 100.0f, zPos));
			
			 float r = 0.5f + (float)(rand() % 129) / 128.0f;
			 float g = 0.5f + (float)(rand() % 129) / 128.0f;
			 float b = 0.5f + (float)(rand() % 129) / 128.0f;
			 l.setColour(Vector4(r, g, b, 1.0f));
			
			 float radius = (RAW_WIDTH * HEIGHTMAP_X / rowLenth);
			 l.setRadius(radius);
			 l.setMesh(lightMesh);

		}
	}


}

void DeferredRenderer::generateBuffers()
{
	glGenFramebuffers(1, &m_Gbuffer);
	glGenFramebuffers(1, &m_lightFBO);
}

void DeferredRenderer::generateAllTextures()
{
	

	generateScreenTexture(m_GDepth, true);
	generateScreenTexture(m_GColour);
	generateScreenTexture(m_GNormal);
	generateScreenTexture(m_lightEmissive);
	generateScreenTexture(m_lightSpecular);

	
}

void DeferredRenderer::generateScreenTexture(GLuint & into, bool depth)
{
	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();
	
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	


	glTexImage2D(GL_TEXTURE_2D, 0,
		depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8,
		width, height, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);


}

void DeferredRenderer::attachTextures()
{
	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	// Set up Gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_Gbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GColour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GDepth, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GNormal, 0);

	// Which buffers to draw to.
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;

	}

	// Set up light buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightEmissive, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightEmissive, 0);
	glDrawBuffers(2, buffers);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);


}

void DeferredRenderer::fillBuffers()
{
	// Bind the G buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_Gbuffer);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_parentRenderer->setCurrentShader(m_sceneShader);

	GLuint diffuseLoc	= glGetUniformLocation(m_sceneShader->getProgram(), "diffuseTex");
	GLuint bumpLoc		= glGetUniformLocation(m_sceneShader->getProgram(), "bumpTex");
	
	glUniform1i(diffuseLoc, TextureUniforms::Diffuse);
	glUniform1i(bumpLoc, TextureUniforms::Bump);

	m_parentRenderer->changeProjection(Projection::Perspective);
	// Change model matrix here?
	m_parentRenderer->updateShaderMatrices();

	// draw stuff here


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::drawLights()
{
	// Bind our lighting FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	m_parentRenderer->setCurrentShader(m_lightShader);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Check how this blending works?
	glBlendFunc(GL_ONE, GL_ONE);


	// Setting up Depth and Normal textures
	GLuint depthTexLoc	= glGetUniformLocation(m_lightShader->getProgram(), "depthTex");
	GLuint normTexLoc	= glGetUniformLocation(m_lightShader->getProgram(), "normTex");

	glUniform1i(depthTexLoc, Depth);
	glUniform1i(normTexLoc, Normal);
	glActiveTexture(GL_TEXTURE0 + TextureUniforms::Depth);
	glBindTexture(GL_TEXTURE_2D, TextureUniforms::Depth);

	glActiveTexture(GL_TEXTURE0 + TextureUniforms::Normal);
	glBindTexture(GL_TEXTURE_2D, TextureUniforms::Normal);


	// Setting up camera and pixel size
	GLuint cameraPosLoc = glGetUniformLocation(m_lightShader->getProgram(), "cameraPos");
	GLuint pixelSizeLoc = glGetUniformLocation(m_lightShader->getProgram(), "pixelSize");
	glUniform3fv(cameraPosLoc, 1, (float*)&m_parentRenderer->getCamera()->GetPosition());

	int width = m_parentRenderer->getWidth();
	int height = m_parentRenderer->getHeight();

	glUniform2f(pixelSizeLoc, 1.0f / width, 1.0f / height);

	float heightY = 10;
	float heightX = 16;
	float heightZ = 16;

	Vector3 translate = Vector3(
		(heightY*heightX / 2),
		500,
		(heightY*heightX / 2));

	Matrix4 pushMatrix = Matrix4::Translation(translate);
	Matrix4 popMatrix  = Matrix4::Translation(-translate);

	float rotation = 10;
	for (int x = 0; x < rowLenth; ++x) {
		for (int z = 0; z < rowLenth; ++z) {
			Light &l = m_lights[(x*rowLenth) + z];
			float radius = l.getRadius();

			Matrix4 modelMatrix =
				pushMatrix * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
				popMatrix *
				Matrix4::Translation(l.getPosition()) *
				Matrix4::Scale(Vector3(radius, radius, radius));

			m_parentRenderer->setModelMatrix(modelMatrix);

			l.setPosition(modelMatrix.GetPositionVector());
		
			m_parentRenderer->setShaderLight(m_lightShader, l);
			m_parentRenderer->updateShaderMatrices();

			Vector3 cameraPos = m_parentRenderer->getCamera()->GetPosition();

			float cameraDis = (l.getPosition() - cameraPos).Length();

			if (cameraDis < radius) {
				glCullFace(GL_FRONT);
			}
			else {
				glCullFace(GL_BACK);
			}

			l.getMesh()->draw();
		}
	}

	// Back to default settings
	
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_parentRenderer->setClearColour(Vector4(0.3, 0.5, 0.4, 1));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

}

void DeferredRenderer::combineBuffers(){
	m_parentRenderer->setCurrentShader(m_combineShader);

	m_parentRenderer->changeProjection(Projection::Orthographic);

	m_parentRenderer->updateShaderMatrices();

	GLuint diffuseTexLoc = glGetUniformLocation(m_combineShader->getProgram(), "diffuseTex");
	GLuint emissiveTexLoc = glGetUniformLocation(m_combineShader->getProgram(), "emissiveTex");
	GLuint specularTexLoc = glGetUniformLocation(m_combineShader->getProgram(), "specularTex");

	glActiveTexture(GL_TEXTURE0 + TextureUniforms::Diffuse);
	glBindTexture(GL_TEXTURE_2D, m_GColour);

	glActiveTexture(GL_TEXTURE0 + TextureUniforms::LightEmissive);
	glBindTexture(GL_TEXTURE_2D, m_lightEmissive);


	glActiveTexture(GL_TEXTURE0 + TextureUniforms::LightSpecular);
	glBindTexture(GL_TEXTURE_2D, m_lightSpecular);

	// bind first?
	m_quad->setTextureType(Texture_2D);
	m_quad->bindTexture();
	m_quad->draw();

	glUseProgram(0);

}

