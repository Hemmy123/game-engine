#version 330 core

// Perlin noise uniforms
uniform float height;
uniform int permutationSize;

uniform int octaves;
uniform float frequency;
uniform float persistance;
uniform float perlinTime;

uniform sampler1D permTexture;

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

float weights[8] = float[8](
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0);

vec3 randomVectors[12] = vec3[12](
	vec3(1.0, 1.0, 0.0),	vec3(-1.0, 1.0, 0.0),	vec3(1.0, -1.0, 0.0),	vec3(-1.0, -1.0, 0.0),
	vec3(1.0, 0.0, 1.0),	vec3(-1.0, 0.0, 1.0), 	vec3(1.0, 0.0, -1.0),	vec3(-1.0, 0.0, -1.0),
	vec3(0.0, 1.0, 1.0),	vec3(0.0, -1.0, 1.0),	vec3(0.0, 1.0, -1.0),	vec3(0.0, -1.0, -1.0)
);

vec3 surroundingPoints[8] = vec3[8](
	vec3(0.0,0.0,0.0), vec3(0.0,0.0,0.0),
	vec3(0.0,0.0,0.0), vec3(0.0,0.0,0.0),
	vec3(0.0,0.0,0.0), vec3(0.0,0.0,0.0),
	vec3(0.0,0.0,0.0), vec3(0.0,0.0,0.0) );

int permSize = 256;

int[256] permArr = int[256] (174,79,155,62,161,6,83,201,192,202,134,14,125,
                          154,34,53,27,170,237,108,127,236,175,158,4,233,
                          120,102,251,20,194,153,8,51,123,21,11,96,66,111,
                          83,200,192,108,15,111,85,6,226,122,194,124,195,4
                          ,134,208,166,117,190,145,15,169,196,194,229,59,200,
                          133,237,64,8,110,193,227,103,149,78,212,119,96,148,
                          51,62,140,53,256,123,183,46,114,84,131,15,164,47,
                          235,21,214,108,255,166,55,230,179,115,172,116,
                          123,159,156,62,152,25,26,119,227,64,154,80,84,
                          148,49,164,57,236,41,221,55,98,83,232,238,48,167,
                          255,77,228,82,71,170,200,3,114,120,152,186,8,163,
                          169,240,50,142,225,53,27,134,241,87,89,40,119,37,
                          70,239,239,4,239,245,253,220,178,251,112,160,9,201,
                          134,112,89,68,60,35,145,126,129,125,118,190,107,86,
                          234,255,191,105,193,152,20,154,188,22,26,177,191,183,
                          83,144,8,90,200,197,224,10,45,71,200,187,245,226,
                          206,122,26,207,253,50,125,161,65,17,53,203,189,83,
                          202,189,16,16,81,171,210,131,85,78,256,117,256,49,
                          110,84,16,19,155,41,47,215,93,177);

void surroundingPointsOf(vec3 point){
	vec3 p0 = vec3(floor(point.x), floor(point.y), floor(point.z));
	vec3 p1 = p0 + vec3(1.0, 0.0, 0.0);
	vec3 p2 = p0 + vec3(0.0, 1.0, 0.0);
	vec3 p3 = p0 + vec3(1.0, 1.0, 0.0);
	vec3 p4 = p0 + vec3(0.0, 0.0, 1.0);
	vec3 p5 = p0 + vec3(1.0, 0.0, 1.0);
	vec3 p6 = p0 + vec3(0.0, 1.0, 1.0);
	vec3 p7 = p0 + vec3(1.0, 1.0, 1.0);

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
	surroundingPointsOf(point);
	for (int i = 0; i < 8; ++i) {
		int x = int((surroundingPoints[i].x));
		int y = int((surroundingPoints[i].y));
		int z = int((surroundingPoints[i].z));
		
		int px = abs(x % permSize);
		int py = abs(y % permSize);
		int pz = abs(z % permSize);

		
		 // int perm1 = int(texelFetch(permTexture, py, 0).r);
		 // int perm2 = int(texelFetch(permTexture, px + perm1, 0).r);
		 // int perm3 = int(texelFetch(permTexture, pz + perm2, 0).r);

		int perm1 = permArr[py];
		int perm2 = permArr[px + perm1];
		int perm3 = permArr[pz + perm2];

		int gradIndex = perm3 % 11;
		
		
		vec3 gradientVector = randomVectors[gradIndex];
		vec3 cornerToPoint = surroundingPoints[i] - point ;
		
		float dotProduct = dot(cornerToPoint,gradientVector);
		weights[i] = dotProduct;
	}
	
}


float fadeFunction(float t){
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
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
		float s = float(permSize) / freq;

		vec3 pos = vec3(point.x/s, point.y/s, point.z/s);
		total += noiseAtVec(pos) * amplitude;
		
		amplitude *= pers;
		freq *= 2.0;
		
	}
	return total;
	
}



vec3 calculateFinalPosition(vec3 calVec){
	//vec3 pos = vec3(calVec.x, calVec.y , calVec.z);
	//vec3 pos = vec3(position.x,	position.z , 0);

	float noiseVal = noiseAt(calVec, octaves, frequency, persistance );
	
	
	float finalHeight = noiseVal * height;


	vec3 finalPos = vec3(position.x, finalHeight, position.z);

	return finalPos;
}

void main(){
	

	vec3 pos = vec3(position.x, position.z, perlinTime);

	
	vec3 finalPosition = calculateFinalPosition(pos);

	// OUT.colour 			= colour;

	OUT.colour = vec4(finalPosition,1);


 	OUT.texCoord 		= (textureMatrix* vec4(texCoord, 0.0, 1.0)).xy;

 	mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));

 	OUT.normal 		= normalize(normalMatrix * normalize(normal));
 	OUT.worldPos	= (modelMatrix * vec4(finalPosition,1)).xyz;

	gl_Position = ( (projMatrix * viewMatrix * modelMatrix) * vec4(finalPosition, 1.0));

}

