/*
 *  Simple fragment shader for the first
 *  texture example.  Just look up the texture
 *  value at the current text coordinate
 */

#version 330 core

in vec2 texCoord1;
in vec2 texCoord2;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main(void) {

	gl_FragColor = texture(tex1, texCoord1) + texture(tex2, texCoord2);

}