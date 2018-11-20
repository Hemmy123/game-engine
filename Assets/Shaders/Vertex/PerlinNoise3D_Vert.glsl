#version 330 core

// Perlin noise uniforms
uniform float height;
uniform int permutationSize;

uniform int octaves;
uniform float frequency;
uniform float persistance;
uniform float perlinTime;

layout(std140) uniform MyBlock
{
  int perm[500];
};


// normal rendering uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;


// input
in   vec3  position;
in   vec4  colour;
in   vec3  normal;      
in   vec2  texCoord;

 out  Vertex {
     vec4  colour;
     vec2  texCoord;
     vec3  normal;
     vec3  worldPos;
 } OUT;

float weights[8] = float[8](0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);

vec3 randomVectors[12] = vec3[12](
	vec3(1,1,0),	vec3(-1,1,0),	vec3(1,-1,0),	vec3(-1,-1,0),
	vec3(1,0,1),	vec3(-1,0,1), 	vec3(1,0,-1),	vec3(-1,0,-1),
	vec3(0,1,1),	vec3(0,-1,1),	vec3(0,1,-1),	vec3(0,-1,-1)
);

vec3 surroundingPoints[8] = vec3[8](
	vec3(0,0,0), vec3(0,0,0),
	vec3(0,0,0), vec3(0,0,0),
	vec3(0,0,0), vec3(0,0,0),
	vec3(0,0,0), vec3(0,0,0) );


void surroundingPointsOf(vec3 point){
	vec3 p0 = vec3(floor(point.x), floor(point.y), floor(point.z));
	vec3 p1 = p0 + vec3(1,0,0);
	vec3 p2 = p0 + vec3(0,1,0);
	vec3 p3 = p0 + vec3(1,1,0);
	vec3 p4 = p0 + vec3(0,0,1);
	vec3 p5 = p0 + vec3(1,0,1);
	vec3 p6 = p0 + vec3(0,1,1);
	vec3 p7 = p0 + vec3(1,1,1);

	surroundingPoints[0] = p0;
	surroundingPoints[1] = p1;
	surroundingPoints[2] = p2;
	surroundingPoints[3] = p3;
	surroundingPoints[4] = p4;
	surroundingPoints[5] = p5;
	surroundingPoints[6] = p6;
	surroundingPoints[7] = p7;
}

void calculateSurroundingWeights(vec3 point){

	for (int i = 0; i < 8; ++i) {
		int x = int((surroundingPoints[i].x));
		int y = int((surroundingPoints[i].y));
		int z = int((surroundingPoints[i].z));
		
		int px = abs(x %permutationSize);
		int py = abs(y %permutationSize);
		int pz = abs(z %permutationSize);

		
		// using all x y z coords to get get unique perm number
		int perm1 = perm[py];
		int perm2 = perm[px + perm1];
		int perm3 = perm[pz + perm2];
		int gradIndex = int(mod(perm3,11));
		
		
		vec3 gradientVector = randomVectors[gradIndex];
		vec3 cornerToPoint = surroundingPoints[i] - point ;
		
		float dotProduct = dot(cornerToPoint,gradientVector);
		weights[i] = dotProduct;
	}
	
}


float fadeFunction(float t){
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float noiseAtVec(vec3 point){
	float intX = floor(point.x);
	float intY = floor(point.y);
	float intZ = floor(point.z);
	
	// Decimal part of float
	float relativeX = point.x - intX;
	float relativeY = point.y - intY;
	float relativeZ = point.z - intZ;

	calculateSurroundingWeights(point);
	float fadeX = fadeFunction(relativeX);
	float fadeY = fadeFunction(relativeY);
	float fadeZ = fadeFunction(relativeZ);


	float bot1 = mix(weights[0], weights[1], fadeX);
	float bot2 = mix(weights[4], weights[5], fadeX);
	float bot = mix(bot1, bot2, fadeZ);

	float top1 = mix(weights[2], weights[3], fadeX);
	float top2 = mix(weights[6], weights[7], fadeX);
	float top = mix(top1, top2, fadeZ);
	
	float finalmix = mix(bot,top,fadeY);

	return finalmix;
}




float noiseAt(vec3 point, int oct, float fre, float pers){
	float freq = fre;
	float amplitude = 1.0;
	float total = 0.0;
	for (int i = 0; i < oct; ++i) {
		float s = float(permutationSize) / freq;

		vec3 pos = vec3(point.x / s, point.y / s, point.z / s);
		total += noiseAtVec(pos) * amplitude;
		
		amplitude *= pers;
		freq *= 2;
		
	}
	return total;
	
}



vec4 calculateFinalPosition(){
	vec3 pos = vec3(position.x, position.y, perlinTime);

	float noiseVal = noiseAt(pos, octaves, frequency, persistance );
	float finalHeight = noiseVal * height;
	vec3 finalPos = vec3(position.x, position.y *finalHeight, position.z);

	return ( (projMatrix * viewMatrix * modelMatrix) * vec4(finalPos, 1.0));
}

void main(){

	OUT.colour 			= colour;
 	OUT.texCoord 		= (textureMatrix* vec4(texCoord, 0.0, 1.0)).xy;

 	mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));

 	OUT.normal 		= normalize(normalMatrix * normalize(normal));
 	OUT.worldPos	= (modelMatrix * vec4(position,1)).xyz;

	vec4 finalPosition = calculateFinalPosition();

	gl_Position = finalPosition;

}

