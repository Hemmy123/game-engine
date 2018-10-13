#version 150 core

uniform sampler2D 	screenTex;
uniform vec2 		screenSize;
uniform float 		time;

in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 colour;

const float weights[5] = float[]( 0.12, 0.22, 0.32, 0.22, 0.12);


void main(void ){
	//float x = time+screenSize.x*IN.texCoord.x;
	float y = time+screenSize.y*IN.texCoord.y;
	
	float x = IN.texCoord.x;
	//float y = IN.texCoord.y;

	
	colour = texture( screenTex, IN.texCoord + 0.005*vec2( sin(x),cos(y)) );

	

}


