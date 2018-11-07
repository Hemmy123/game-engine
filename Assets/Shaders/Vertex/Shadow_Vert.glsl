#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mat4 mvp;

in vec3 position;

void main(){
	gl_Position = mvp * vec4(position, 1.0);
}