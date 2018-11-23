#version 430 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(triangles) in;
layout(line_strip, max_vertices = 3) out;
//layout(triangle_strip, max_vertices = 3) out;

in Vertex {
     vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
} IN[];



 out  Vertex {
     vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
 } OUT;


void main(){
	for (int i =0 ; i< gl_in.length(); ++i){
 		gl_Position = gl_in[i].gl_Position;
		OUT.texCoord = IN[i].texCoord;
		OUT.colour = IN[i].colour;
		OUT.normal = IN[i].normal;
		OUT.worldPos = IN[i].worldPos;
 		EmitVertex();
	}
 	EndPrimitive();

}