/*
 *  Simple fragment sharder for laboratory two
 */

in vec3 normal;
in vec4 position;
uniform vec3 eye;

void main() {
	vec3 N;
	vec3 Lposition = vec3(500.0, 500.0, 800.0);
	vec4 colour = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 Lcolour = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 spotDirection = vec3(500.0, 500.0, 750.0);
	float spotCutoff = 0.85;
	float spotExp = 200.0;
	float spotCos;
	float atten;
	vec3 H;
	float diffuse;
	float specular;
	float n = 100.0;
	vec3 L;

	N = normalize(normal);
	L = Lposition - position.xyz;
	L = normalize(L);
	H = normalize(L + eye);
	spotCos = dot(L, normalize(spotDirection));
	if(spotCos < spotCutoff) {
		atten = 0;
	} else {
		atten = pow(spotCos,spotExp);
	}
	diffuse = dot(N,L) * atten;
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,H)),n) * atten;
	}

	gl_FragColor = min(0.3*colour + diffuse*colour*Lcolour + Lcolour*specular, vec4(1.0));
	gl_FragColor.a = colour.a;
}