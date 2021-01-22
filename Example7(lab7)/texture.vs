/*
 *  Simple vertex shader for the first
 *  texture example.
 */

#version 330 core

in vec4 vPosition;
in vec2 vTexture;

uniform mat4 modelView;

out vec2 texCoord;

void main(void) {
	
	vec2 newTexture;
	//vec2 pivot = vTexture;
	vec2 pivot = vPosition.xy;
	float rotation = 120;
	float sine = sin(rotation);
    float cosine = cos(rotation);

    newTexture = pivot;
    newTexture.x = newTexture.x * cosine - newTexture.y * sine;
    newTexture.y = newTexture.x * sine + newTexture.y * cosine;
    


	gl_Position = modelView * vPosition;
	//texCoord = vTexture;
	//texCoord = vTexture*4;
	texCoord = newTexture;

}