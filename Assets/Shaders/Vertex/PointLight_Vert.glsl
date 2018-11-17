#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform mat4 mvp;

in vec3 position;
out mat4 inverseProjView

void main(void) {
	gl_Position = mvp * vec4(position, 1.0);

	// Should do on the CPU because expesive here?
	inverseProjView = inverse(projMatrix*viewMatrix);
}
