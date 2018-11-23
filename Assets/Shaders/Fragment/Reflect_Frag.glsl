#version 430 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 outColour;

void main(void){
	int blend = 1;

	vec4 	diffuse 	= texture(diffuseTex, IN.texCoord) ;		// The water texture
	vec3 	incident	= normalize(IN.worldPos - cameraPos);				// vertex pos to camera pos
	float 	dist		= length(lightPos - IN.worldPos);
	float 	atten		= 1.0 - clamp(dist / lightRadius, 0.2, 1.0);
	vec4	reflection 	= texture(cubeTex,
						  reflect(incident, normalize(IN.normal)) );
	outColour = (lightColour * diffuse * atten) * (diffuse/blend + reflection*blend);
	outColour.a = 0.85;

	//outColour = IN.colour;

}
