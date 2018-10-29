////========================================
// Class: PostProcessor
// Author: Hemmy
// Date: 29/07/2018
// Description:
//
// ========================================

#include "PostProcessor.h"


void PostProcessor::drawSceneToFBO(bool trigger)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);


	clearBuffers();
	setCurrentShader(m_sceneShader);

	m_projMatrix = m_persp;
	updateShaderMatrices(m_currentShader);
	checkErrors();

	for (auto iter : m_opaqueObjects) { drawRenderObject(*iter); }
	for (auto iter : m_transparentObjects) { drawRenderObject(*iter); }


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void PostProcessor::generateFBOTexture(){
	// Generate depth texture
	glGenTextures(1, &m_buffDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_buffDepthAttachment);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	
	// Note:
	// GL_DEPTH24_STENCIL8 and GL_DEPTH_STENCIL because it's a depth texture
	
	
	
	glGenTextures(1, &m_buffColourAttachment);
	glBindTexture(GL_TEXTURE_2D, m_buffColourAttachment);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 	// clamping to make sure no sampling happens that
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// might distort the edges. (Try turning htis off?)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	
	// Generate FBOs
	glGenFramebuffers(1, &m_sceneFBO);
	glGenFramebuffers(1, &m_processFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);
	
	// Attaching attachments to sceneFBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 	GL_TEXTURE_2D, m_buffDepthAttachment, 	0);		// Depth attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, 	GL_TEXTURE_2D, m_buffDepthAttachment, 	0);		// Stencil attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 	GL_TEXTURE_2D, m_buffColourAttachment,0);		// Colour attackment (only one?)
	
	
	// Checking if FBO attachment was successful
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER)  != GL_FRAMEBUFFER_COMPLETE || !m_buffDepthAttachment  || !m_buffColourAttachment){
		
		std::cout << "FBO Attachment failed "<<  std::endl;
		
		return;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}
