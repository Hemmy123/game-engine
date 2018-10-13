
#version 330 core

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

uniform sampler2D textureSample;


out vec4 colour;

void main(){
	
	colour = texture(textureSample, IN.texCoord);
	//colour = vec4(1,0,0,1);
}



