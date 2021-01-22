/*
 *  Simple vertex shader for the first
 *  texture example.
 */

#version 330 core

in vec4 vPosition;
in vec2 vTexture;

uniform mat4 modelView;
uniform float time;

out vec2 texCoord1;
out vec2 texCoord2;

void main(void) {

	mat2 m = mat2(vec2(cos(time), sin(time)),
			    vec2(-sin(time), cos(time)));
	gl_Position = modelView * vPosition;
	texCoord1 = vTexture * m;
	texCoord2 = vTexture * transpose(m);

}