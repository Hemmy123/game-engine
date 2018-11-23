#version 330
 
uniform sampler2D leftTex;
uniform sampler2D rightTex;

uniform sampler2D leftDepthTex;
uniform sampler2D rightDepthTex;

 
in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN; 

out vec4 outColour;
 
void main(void)
{

	float leftDepth 	= texture(leftDepthTex, IN.texCoord).r;
	float rightDepth 	= texture(rightDepthTex, IN.texCoord).r;

	
	if(leftDepth == 1 && rightDepth == 1){
		discard;
	} else{
		// Shader from https://r3dux.org/2011/05/anaglyphic-3d-in-glsl/


		vec4 leftFrag = texture(leftTex, IN.texCoord);
		leftFrag = vec4(1.0, leftFrag.g, leftFrag.b, 1.0); // Left eye is full red and actual green and blue
		
		vec4 rightFrag = texture(rightTex, IN.texCoord);
		rightFrag = vec4(rightFrag.r, 1.0, 1.0, 1.0); // Right eye is full green and blue and actual red
	 
		// Multiply left and right components for final ourput colour
		outColour = vec4(leftFrag.rgb * rightFrag.rgb, 1.0); 
	}
		

	



}