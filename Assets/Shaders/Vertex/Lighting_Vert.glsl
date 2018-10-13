#version 330 core
layout (location = 0)

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 mvp;

out Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT;

void main(void){
	OUT.worldPos 		= (modelMatrix * vec4(position,1)).xyz;

	//OUT.worldPos	 	= position;
	mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));
	
	OUT.normal 			= normalize(normalMatrix * normalize(normal));
	OUT.texCoord 		= texCoord;
//	OUT.worldPos 		= (modelMatrix * vec4(position,1)).xyz;
//	OUT.colour 			= colour;
	gl_Position 		= mvp * vec4(position, 1.0);

}
