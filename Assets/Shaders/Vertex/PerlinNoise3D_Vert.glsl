#version 330 core

// Perlin noise uniforms
uniform float height;
// uniform int permutationSize;

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


int permSize = 1024;

// Had to hard code in random numbers because I couldn't get the 1D texture to work :( 
int[1024] permArr = int[1024] (372,183,738,46,723,964,66,471,429,458,837,894,892,22,138,599,1012,999,994,114,41,436,483,189,598,
	62,781,824,28,942,198,558,865,345,530,955,138,64,674,331,572,185,430,165,1000,404,232,193,588,241,1004,871,337,793,102,351,555,
	657,468,359,371,65,260,380,644,738,366,497,573,93,604,134,78,610,1014,616,862,862,216,635,269,812,508,392,479,290,190,592,800,
	941,11,553,952,468,588,874,797,293,227,978,806,223,370,3,292,374,986,922,413,847,307,974,236,576,447,872,961,680,432,14,210,
	49,261,76,729,434,416,722,564,935,813,670,685,91,573,490,144,878,416,41,687,929,122,302,422,342,587,580,434,935,398,509,268,
	423,191,235,841,207,72,866,1009,858,280,333,910,381,999,100,354,880,789,677,917,538,709,79,527,691,234,148,885,344,919,422,
	542,553,325,605,676,745,275,384,664,674,214,791,912,69,688,390,781,589,108,562,110,230,321,689,331,133,670,196,78,541,413,804,
	13,899,311,880,271,75,298,881,482,538,328,548,310,146,296,481,500,101,60,394,744,839,187,629,918,952,889,984,384,455,26,757,
	182,292,828,771,720,127,848,732,847,95,502,60,171,626,437,54,615,194,935,912,673,938,1019,221,35,912,529,792,599,794,237,784,
	438,748,244,39,849,72,423,343,938,614,295,980,869,392,447,472,193,528,932,342,57,557,167,316,489,7,78,585,302,416,109,396,
	159,150,894,571,465,931,906,961,713,579,936,329,379,157,514,618,32,901,977,297,536,655,355,630,188,629,502,285,120,297,355,
	436,78,412,49,793,316,1006,682,875,467,714,473,733,591,93,748,531,718,728,86,47,634,783,292,136,999,14,990,670,282,667,910,261,
	1024,674,714,1007,865,572,941,891,375,652,159,850,913,192,156,197,157,717,621,998,819,654,995,149,211,170,154,958,60,919,132,933,
	1001,53,259,635,283,322,174,5,294,510,183,158,27,61,590,847,93,894,127,283,751,731,371,831,681,681,144,291,177,65,369,210,148,285,
	548,151,211,929,141,347,685,335,122,870,1008,399,719,148,548,372,301,368,665,593,991,339,820,76,135,671,716,706,720,557,995,172,
	134,518,69,189,149,152,599,999,962,136,741,741,429,848,623,42,62,406,695,531,534,190,72,181,723,563,240,610,631,572,280,348,592,
	488,628,21,857,40,607,822,216,567,12,648,71,124,651,594,356,773,279,846,160,790,427,682,680,480,560,88,469,566,330,979,935,304,
	658,887,796,966,184,428,456,904,473,568,950,490,504,1021,683,831,915,543,854,78,297,824,90,250,145,768,846,563,608,60,213,109,342,
	290,227,24,369,383,892,195,736,1,951,15,847,158,426,943,314,681,168,184,851,801,941,231,637,308,725,824,544,245,681,844,720,875,596,
	910,366,7,547,264,361,188,865,617,185,606,186,597,980,302,620,1019,214,633,876,392,908,845,455,283,474,565,61,771,284,1015,1018,
	384,688,242,508,739,740,74,832,11,384,678,806,596,650,310,171,139,844,303,183,124,413,792,133,815,25,653,471,203,807,918,740,168,
	725,512,386,500,476,125,542,865,591,797,932,225,272,252,611,361,790,705,475,769,796,931,841,20,843,464,671,796,718,989,695,662,650,
	255,527,612,287,419,912,960,887,595,1022,763,151,340,723,95,287,688,575,694,174,670,218,664,90,297,982,644,692,1019,802,65,186,324,
	925,73,83,681,570,707,242,26,698,930,927,67,149,442,575,48,116,88,79,952,812,239,64,10,170,260,277,542,658,114,464,580,414,460,688,
	445,675,314,563,210,883,271,802,883,861,941,258,405,287,264,847,472,46,232,145,938,561,559,811,286,784,129,547,611,759,111,314,130,
	98,1018,102,119,735,825,22,1016,740,843,706,380,762,967,733,462,707,798,341,880,425,224,743,486,709,800,309,501,791,219,36,623,1006,
	128,593,5,905,342,849,5,771,378,866,702,191,949,43,838,154,384,506,193,638,132,744,657,500,170,1021,770,530,372,291,182,499,209,578,
	827,176,691,901,254,787,729,707,832,362,110,395,513,190,875,980,9,813,377,1015,76,265,149,322,229,946,694,209,299,150,742,280,829,581,
	926,287,8,268,444,1019,128,872,570,137,797,406,82,962,663,448,619,23,644,892,203,802,123,174,147,20,807,538,690,431,679,1010,948,556,
	320,841,131,451,117,790,80,784,50,200,428,378,798,822,148,255,1012,100,423,438,714,169,887,374,563,61,871,264,403,691,629,29,741,
	120,853,751,915,164,458,242,609,369,802,733,30,381,958,13,594,93,524,763,48,769,871,709,761,703,152,542,954,576,816,875,459,137,
	161,953,376,958,127,352,637,1010,102,338,128,834,213,620,1002,380,940);

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
	
// --------testing

	vec3 tempPos = vec3(0,0,perlinTime);
	vec3 pos = vec3(position.x, position.z, perlinTime);

// ------------

	// vec3 pos = vec3(position.x, position.z +perlinTime, perlinTime);

	
	vec3 finalPosition = calculateFinalPosition(pos);

	// OUT.colour 			= colour;

	OUT.colour = vec4(finalPosition,1);


 	OUT.texCoord 		= (textureMatrix* vec4(texCoord, 0.0, 1.0)).xy;

 	mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));

 	OUT.normal 		= normalize(normalMatrix * normalize(normal));
 	OUT.worldPos	= (modelMatrix * vec4(finalPosition,1)).xyz;

	gl_Position = ( (projMatrix * viewMatrix * modelMatrix) * vec4(finalPosition, 1.0));

}

