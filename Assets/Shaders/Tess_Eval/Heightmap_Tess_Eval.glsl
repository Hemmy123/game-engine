#version 430 core

layout(triangles,equal_spacing, ccw) in ;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in Vertex {
	 vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
} IN[];


out Vertex{
	 vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
}OUT;



void main(){

	vec3 p0  = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
	vec3 p1  = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
	vec3 p2  = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

	vec4 c0  = gl_TessCoord.x * IN[0].colour;
	vec4 c1  = gl_TessCoord.y * IN[1].colour;
	vec4 c2  = gl_TessCoord.z * IN[2].colour;

	vec2 t0  = gl_TessCoord.x * IN[0].texCoord;
	vec2 t1  = gl_TessCoord.y * IN[1].texCoord;
	vec2 t2  = gl_TessCoord.z * IN[2].texCoord;


	vec3 combinedPos = p0+p1+p2;
	vec4 colourMix = c0 +c1 +c2;
	vec2 texCoordMix = t0+t1+t2;

	vec4 worldPos = modelMatrix * vec4(combinedPos, 1);


	// Passing out stuff
	gl_Position = projMatrix *viewMatrix * worldPos;
	OUT.colour = colourMix;
	OUT.texCoord = texCoordMix;




}