/*
 *  Simple fragment sharder for Example 10
 */

#version 330 core

in vec3 normal;
in vec3 position;
in vec2 tc;
uniform samplerCube tex;
uniform vec4 colour;
uniform vec3 Eye;
uniform vec3 light;
uniform vec4 material;


int num = 1235;
int a = 141;
int c = 28411;
int m = 134456;

float rand() {
	float f;
	num = (a*num+c) % m;
	f = (num+0.0)/m;
	return((f-0.5)*2.0);
}


void main() {

	num = int((normal.x*normal.y + normal.x*normal.z) * 123456);
	num = num % m;
	float R0;
	float r;
	
	vec3 N = normal; 
	N = normalize(N);
	
	float nc = 1.5;
	
	int check = 0;
	float storeRandSum = 0.0;
	vec4 vecSum;
	while(check < 499) {
		vec3 randVec = vec3(rand(), rand(), rand());
		float temp = dot(randVec, N);
		
		if(temp > 0){
			vec4 textureLookup = texture(tex, randVec);
			vec4 weightedLookup = temp*textureLookup;
			vecSum = vecSum + weightedLookup;
			storeRandSum = storeRandSum + temp;
			check++;
		}
	}
	
	vecSum = vecSum / storeRandSum;
	
	vec3 reflected = reflect( normalize(position - Eye), N);
	vec3 refracted = refract( normalize(position - Eye), N, 1.50);
	
	R0 = pow((nc - 1),2)/pow((nc + 1),2);
	r = R0 + (1 - R0) * pow((1 - (dot(normalize(Eye), N)) ),5);
		
	//used for part 1 for combining reflection and refraction
	//gl_FragColor = r*texture(tex, reflected) + (1-r)*texture(tex, refracted);
	
	//just reflection 
	//gl_FragColor = texture(tex,reflected);
	gl_FragColor = vecSum;
	
	
	
	

}