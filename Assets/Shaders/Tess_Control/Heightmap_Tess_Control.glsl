#version 430 core

layout(vertices = 3) out;

in Vertex {
	 vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
}IN[];

out Vertex{
	 vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
}OUT[];


void main(){

	gl_TessLevelInner[0] = 3;
	gl_TessLevelOuter[0] = 3;
	gl_TessLevelOuter[1] = 3;
	gl_TessLevelOuter[2] = 3;


	// Passing out stuff
	OUT[gl_InvocationID].texCoord 	= IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].colour 	= IN[gl_InvocationID].colour;
	OUT[gl_InvocationID].normal 	= IN[gl_InvocationID].normal;
	OUT[gl_InvocationID].worldPos 	= IN[gl_InvocationID].worldPos;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	

}