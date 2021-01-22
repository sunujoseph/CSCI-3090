/*
 *  Simple fragment sharder for laboratory two
 */

in vec3 normal;
in vec4 position;   
in vec4 Idiff;
in vec4 Iamb;
in vec4 Ispec;
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
	vec3 E = normalize(L); 
	L = normalize(L);
	H = normalize(L + eye);
	
	
	vec3 R = normalize(reflect(L,N));
	
	spotCos = dot(L, normalize(spotDirection));
	if(spotCos < spotCutoff) {
		atten = 0;
	} else {
		atten = pow(spotCos,spotExp);
	}
	//diffuse = dot(N,L) * atten;
	//diffuse = gl_FrontLightProduct[0].diffuse * max(dot(N, L), 0.1);
	//diffuse = max(diffuse, 0.1)
	diffuse = max(dot(N, L), 0.1);

	
	specular = pow(max(0.0, dot(N,H)),n) * atten;
	//specular = pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);

	//gl_FragColor = Ispec + + diffuse + Idiff + Iamb;
	//gl_FragColor = min(0.3*colour + diffuse*colour*Lcolour + Lcolour*specular, vec4(1.0));
	gl_FragColor = min(Iamb + diffuse*colour*Lcolour + Lcolour*Ispec, vec4(1.0));
	//gl_FragColor = vertex_color;   
	//gl_FragColor.a = colour.a;
	
}