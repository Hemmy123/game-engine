#version 330 core

uniform sampler2D diffuseTex;

uniform vec3 	cameraPos;
uniform vec4 	lightColour;
uniform vec3 	lightPos;
uniform float	lightRadius;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 outColour;

void main(void) {
	

	vec3	incident	= normalize(lightPos	- IN.worldPos);
	vec3	viewDir		= normalize(cameraPos 	- IN.worldPos);
	vec3	halfDir 	= normalize(incident 	+ viewDir);

	float	dist		= length(lightPos - IN.worldPos);
	float 	atten		= 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	float 	lambert		= max(0.0, dot(incident, IN.normal));
	float 	rFactor		= max(0.0, dot(halfDir, IN.normal));
	float 	sFactor		= pow(rFactor, 50.0);

	vec4 	texCol		= texture(diffuseTex, IN.texCoord);

	vec3 	ambient  	= texCol.rgb   * lightColour.rgb * 0.5;
	
	vec3  	diffuse     = texCol.rgb   * lightColour.rgb * lambert * atten;


	//vec3  	diffuse     = texCol.rgb   * lightColour.rgb * lambert ;

	vec3  	specular    = lightColour.rgb  * sFactor * atten;

	vec3	control 	= texCol.rgb;
	
	//outColour       	= vec4(ambient  , texCol.a);
	//outColour       	= vec4(diffuse  , texCol.a);
	//outColour       	= vec4(specular , texCol.a);

	// outColour       	= vec4(control , texCol.a);


	outColour       	= vec4(ambient + diffuse + specular , texCol.a);


	
}
