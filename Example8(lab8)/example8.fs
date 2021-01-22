/*
 *  Simple fragment shader for the first
 *  texture example.  Just look up the texture
 *  value at the current text coordinate
 */

#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec4 position;

uniform sampler2D tex;
uniform vec3 eye;

void main(void) {

	vec4 colour = texture(tex, texCoord);

	vec3 N;
	vec3 L = vec3(1.0, 1.0, 0.0);
	vec4 Lcolour = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 H = normalize(L + vec3(0.0, 0.0, 1.0));
	float diffuse;
	float specular;
	float n = 100.0;

	N = normalize(normal);
	L = normalize(L);
	diffuse = dot(N,L);
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,H)),n);
	}

	gl_FragColor = min(0.3*colour + diffuse*colour*Lcolour + Lcolour*specular, vec4(1.0));
	gl_FragColor.a = colour.a;



}