#version 420

layout(triangles,equal_spacing, ccw) in ;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 normalMatrix;

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



vec3  QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d){
	vec3 p0 	= mix(a,c, gl_TessCoord.x);
	vec3 p1 	= mix(b,d, gl_TessCoord.x);

	return  mix(p0,p1, gl_TessCoord.y);
}

vec2  QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d){
	vec2 p0 	= mix(a,c, gl_TessCoord.x);
	vec2 p1 	= mix(b,d, gl_TessCoord.x);

	return  mix(p0,p1, gl_TessCoord.y);
}


vec4  QuadMixVec4(vec4 a, vec4 b, vec4 c, vec4 d){
	vec4 p0 	= mix(a,c, gl_TessCoord.x);
	vec4 p1 	= mix(b,d, gl_TessCoord.x);

	return  mix(p0,p1, gl_TessCoord.y);
}


void main(){

	


	// vec3 p0  = gl_in[0].gl_Position.xyz;
	// vec3 p1  = gl_in[1].gl_Position.xyz;
	// vec3 p2  = gl_in[2].gl_Position.xyz;
	// vec3 p3  = gl_in[3].gl_Position.xyz;
	// vec3 combinedPos = QuadMixVec3( p0, p1, p2, p3 );

	// vec4 c0  =  IN[0].colour;
	// vec4 c1  =  IN[1].colour;
	// vec4 c2  =  IN[2].colour;
	// vec4 c3  =  IN[3].colour;
	// vec4 combinedCol = QuadMixVec4( c0, c1, c2, c3 );


	// vec2 t0  =  IN[0].texCoord;
	// vec2 t1  =  IN[1].texCoord;
	// vec2 t2  =  IN[2].texCoord;
	// vec2 t3  =  IN[3].texCoord;
	// vec2 combinedTex = QuadMixVec2( t0, t1, t2, t3 );

	// vec3 n0 =  IN[0].normal;
	// vec3 n1 =  IN[1].normal;
	// vec3 n2 =  IN[2].normal;
	// vec3 n3 =  IN[3].normal;
	// vec3 combinedNor = QuadMixVec3( n0, n1, n2, n3 );


	// mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));
	// vec4 worldPos = modelMatrix * vec4(combinedPos, 1);

	// // Passing out stuff
	// gl_Position = projMatrix *viewMatrix * worldPos;
	// OUT.colour 	= combinedCol;
	// OUT.texCoord = combinedTex;
	// OUT.normal = combinedNor * normalMatrix;

	vec3 p0  = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
	vec3 p1  = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
	vec3 p2  = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

	vec4 c0  = gl_TessCoord.x * IN[0].colour;
	vec4 c1  = gl_TessCoord.y * IN[1].colour;
	vec4 c2  = gl_TessCoord.z * IN[2].colour;

	vec2 t0  = gl_TessCoord.x * IN[0].texCoord;
	vec2 t1  = gl_TessCoord.y * IN[1].texCoord;
	vec2 t2  = gl_TessCoord.z * IN[2].texCoord;

	vec3 n0 =  IN[0].normal;
	vec3 n1 =  IN[1].normal;
	vec3 n2 =  IN[2].normal;
	vec3 n3 =  IN[3].normal;


	vec3 combinedNor = QuadMixVec3( n0, n1, n2, n3 );
	mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));



	vec3 combinedPos 	= p0 + p1 + p2;
	vec4 colourMix 		= c0 + c1 + c2;
	vec2 texCoordMix 	= t0 + t1 + t2;

	vec4 worldPos = modelMatrix * vec4(combinedPos, 1);


	// Passing out stuff
	gl_Position = projMatrix *viewMatrix * worldPos;
	OUT.colour = colourMix;
	OUT.texCoord = texCoordMix;
	OUT.normal = normalMatrix * combinedNor;



}