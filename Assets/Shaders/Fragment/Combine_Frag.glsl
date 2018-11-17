#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;

in Vertex{
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void){
	vec3 diffuse 	= texture(diffuseTex, in.texCoord).xyz;
	vec3 light 		= texture(emissiveTex, in.texCoord).xyz;
	vec3 specular 	= texture(specularTex, in.texCoord).xyz;

	fragColour.xyz 	= diffuse * 0.2;	// ambient
	fragColour.xyz += diffuse * light;	// lambert
	fragColour.xyz += specular;			// specular
	fragColour.a 	= 1.0;


}