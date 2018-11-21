#include "TextRenderer.h"



TextRenderer::TextRenderer()
{
	gltInit();
	initTexts();
}


TextRenderer::~TextRenderer()
{

	for (auto textIter : m_textMap) {
		gltDeleteText(textIter.second);
	}

	m_textMap.clear();

	// Destroy glText
	gltTerminate();
}

void TextRenderer::initTexts()
{

	// ----- FPS Text ----- //
	GLTtext *FPSText = gltCreateText();
	gltSetText(FPSText, "The FPS is: ");
	TextSettings FPSSettings;

	FPSSettings.m_position = Vector2(10, 10);
	FPSSettings.m_scale = 2;

	// ----- Controls Text ----- //
	GLTtext *ControlsText = gltCreateText();
	gltSetText(ControlsText, "The controls are... ");
	TextSettings controlsSettings;

	controlsSettings.m_position = Vector2(10, 40);
	controlsSettings.m_scale = 2;



	// ------ Inserting to map ----- //
	m_textMap.insert({ FPS, FPSText });
	m_textMap.insert({ Controls, ControlsText });

	m_textSettings.insert({ FPS, FPSSettings });
	m_textSettings.insert({ Controls, controlsSettings });


}

void TextRenderer::updateText(TextType type, std::string text)
{


	GLTtext* foundText = m_textMap.at(type);
	gltSetText(foundText, text.c_str());
}

void TextRenderer::updateTextSettings(TextType type, TextSettings settings)
{
	 m_textSettings.at(type) = settings;
}

void TextRenderer::renderAllText(GLuint fbo)
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	gltBeginDraw();
	for (auto textIter : m_textMap) {


		Vector2 pos = m_textSettings.at(textIter.first).m_position;
		float scale  = m_textSettings.at(textIter.first).m_scale;
		GLTtext* foundText = m_textMap.at(textIter.first);

		gltDrawText2D(foundText, pos.x, pos.y, scale);

	}
	gltEndDraw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}
