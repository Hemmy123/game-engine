#version 330 core

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

uniform sampler2D diffuseTex;
uniform sampler2D highLightTex;

out vec4 outColour;

void main(){
	
	vec4 sceneColour = texture(diffuseTex, IN.texCoord);
	vec4 highlightColour = texture(highLightTex, IN.texCoord);
	float intensity = 1;

	outColour = sceneColour + (highlightColour* intensity);
}



