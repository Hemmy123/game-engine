#include "TextRenderer.h"



TextRenderer::TextRenderer()
{
	gltInit();

}


TextRenderer::~TextRenderer()
{
}

void TextRenderer::initTexts()
{
}

void TextRenderer::updateText(TextType type, std::string text)
{
	GLTtext* foundText = m_textMap.at(type);

}
