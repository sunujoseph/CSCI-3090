/************************************************
 *
 *             Example 8
 *
 *  This example uses the vase model plus
 *  a texture map loaded from a file to
 *  illustrate the basic process of texture
 *  mapping.
 *
 ************************************************/

#include <Windows.h>
#include <gl/glew.h>
#define GLFW_DLL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include <stdio.h>
#include "tiny_obj_loader.h"
#include <iostream>
#include <FreeImage.h>

#include <FreeImage.h>

#include <stdlib.h>

#include "perlin.h"

#define WIDTH 512
#define HEIGHT 512
unsigned char image[HEIGHT][WIDTH][3];


void makeImage(float f) {
	float vec[2];
	int i, j;
	float dx, dy;
	float n;
	dx = f / WIDTH;
	dy = f / HEIGHT;
	vec[0] = vec[1] = 0.0;
	for (i = 0; i < WIDTH; i++) {
		vec[0] = i * dx;
		for (j = 0; j < HEIGHT; j++) {
			vec[1] = j * dy;
			n = noise2(vec);
			n = 0.5 * n + 0.5;
			image[i][j][0] = 256 * n;
			image[i][j][1] = 256 * n;
			image[i][j][2] = 256 * n;
		}
	}
}

void saveImage(char* filename) {
	int i;
	int j;
	FIBITMAP* bitmap;
	BYTE* bits;
	bitmap = FreeImage_Allocate(WIDTH, HEIGHT, 24);
	for (j = 0; j < HEIGHT; j++) {
		bits = FreeImage_GetScanLine(bitmap, j);
		for (i = 0; i < WIDTH; i++) {
			bits[FI_RGBA_RED] = image[j][i][0];
			bits[FI_RGBA_GREEN] = image[j][i][1];
			bits[FI_RGBA_BLUE] = image[j][i][2];
			bits += 3;
		}
	}
	FreeImage_Save(FIF_PNG, bitmap, filename, PNG_DEFAULT);
}


void main(int argc, char** argv) {
	float f;
	f = 13.0;
	if (argc > 1)

		f = atof(argv[1]);
	printf("f: %f\n", f);
	makeImage(f);
	saveImage("perlin.png");
}