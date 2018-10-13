

#version 330 core

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

uniform sampler2D diffuseTex;


out vec4 colour;

void main(){
	
	colour = texture(diffuseTex, IN.texCoord);
	//colour = vec4(1,0,0,1);
}



