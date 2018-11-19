#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;

uniform sampler2D depthTex;
uniform sampler2D skyboxQuad; // The skybox after it has been rendered to a quad

in Vertex{
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void){
	vec3 diffuse 	= texture(diffuseTex, 	IN.texCoord).xyz;
	vec3 light 		= texture(emissiveTex, 	IN.texCoord).xyz;
	vec3 specular 	= texture(specularTex, 	IN.texCoord).xyz;

	float depth 	= texture(depthTex, IN.texCoord).r;

	// If the depth is infinitely far away, then it's
	// probably the skybox
	if(depth == 1){
		discard;
	} else{

		fragColour.xyz 	= diffuse * 0.2;	// ambient
		fragColour.xyz += diffuse * light;	// lambert
		fragColour.xyz += specular;			// specular
		fragColour.a 	= 1.0;
	}



}