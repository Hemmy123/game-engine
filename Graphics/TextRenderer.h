#pragma once
#include "glText.h"
#include <map>


enum TextType {
	FPS,
	Controls,
};
class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void initTexts();
	void updateText(TextType type, std::string text);

private:
	std::map<TextType, GLTtext*> m_textMap;


};

