
// Sobel Edge Detection Filter
// GLSL Fragment Shader
// Implementation by Patrick Hebron

// Shader from https://gist.github.com/Hebali/6ebfc66106459aacee6a9fac029d0115
#version 330 core

uniform sampler2D	diffuse;
uniform vec2 		pixelSize;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 outColour;

void make_kernel(inout vec4 n[9], vec2 coord)
{
	float w = pixelSize.x;
	float h = pixelSize.y;

	n[0] = texture2D(diffuse, coord + vec2( -w, -h));
	n[1] = texture2D(diffuse, coord + vec2(0.0, -h));
	n[2] = texture2D(diffuse, coord + vec2(  w, -h));
	n[3] = texture2D(diffuse, coord + vec2( -w, 0.0));
	n[4] = texture2D(diffuse, coord);
	n[5] = texture2D(diffuse, coord + vec2(  w, 0.0));
	n[6] = texture2D(diffuse, coord + vec2( -w, h));
	n[7] = texture2D(diffuse, coord + vec2(0.0, h));
	n[8] = texture2D(diffuse, coord + vec2(  w, h));
}

void main(void) 
{
	vec4 n[9];
	make_kernel( n, IN.texCoord );

	vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	outColour = vec4( 1.0 - sobel.rgb, 1.0 );
}