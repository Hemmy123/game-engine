#pragma once

#define GLT_IMPLEMENTATION
#include "glText.h" 
#include <map>
#include "Vector2.h"

enum TextType {
	FPS,
	Controls,
};

struct TextSettings {
	Vector2 m_position;
	float m_scale;
};

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void initTexts();
	void updateText(TextType type, std::string text);
	void updateTextSettings(TextType type, TextSettings settings);

	std::string intToString(int num);

	void renderAllText(GLuint fbo);


private:
	std::map<TextType, GLTtext*>		m_textMap;
	std::map<TextType, TextSettings>	m_textSettings;


};

