#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;

out Vertex{
	vec3 normal;
} OUT;

void main(void){
	vec3 tempPos = position - vec3(0,0,1);
	OUT.normal 	= transpose(mat3(viewMatrix)) * normalize(tempPos);
	
	// vec4 temp2  = projMatrix * vec4(tempPos * 9.9 , 1.0);
	// gl_Position = vec4(temp2.x,-temp2.y,temp2.z,temp2.w);
	gl_Position = projMatrix * vec4(tempPos, 1.0);



}

