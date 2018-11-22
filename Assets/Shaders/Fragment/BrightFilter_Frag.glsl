#version 330 core

// The texture we want to extract the bright parts out of
uniform sampler2D diffuseTex;	

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;


out vec4 outColour;

void main(){
	
	vec4 textureColour =  texture(diffuseTex, IN.texCoord);
	// Sum colours and divide to get average, very basic 'brightness indicator'

	// Values from https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems 
	float brightness = (textureColour.r * 0.2126) + (textureColour.g * 0.7152) + (textureColour.b * 0.0722) ;

	outColour = textureColour * brightness*2;

	
}



