/************************************************
 *
 *             Example Five
 *
 *  The stickman program.  This example shows
 *  how a hierarchical model can be constructed
 *  and displayed using OpenGL
 *
 ************************************************/

#include "Window.h"
#include "PTrace.h"
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
#include <stack>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <memory>
#include <ctime>

#define WIDTH 800
#define HEIGHT 600
#define ASSIGNMENT_PART 0 //0 -> visualization 1, 1 -> visualization 2

float phi, theta, r;
glm::vec3 eye;
glm::vec3 center;
glm::mat4 view;
glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);;
glm::mat4 view_persp;
glm::mat4 model = glm::mat4(1.0f);
std::unique_ptr<Window> window;

// takes in delta time to take framerate into account
void recalc_matrices(float dt)
{
	view = glm::lookAt(eye, center, glm::vec3(0.0f, 0.0f, 1.0f));
	view_persp = projection * view;

	// rotate the model around the center of the paths
	model = glm::translate(model, center);
	model = glm::rotate(model, -0.5f * dt, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -center);
}

// key actions
// takes in delta time to take framerate into account
void update(float dt)
{
	if (window->GetKey(GLFW_KEY_W) || window->GetKey(GLFW_KEY_A) || window->GetKey(GLFW_KEY_S) || window->GetKey(GLFW_KEY_D))
	{
		if (window->GetKey(GLFW_KEY_W))
		{
			theta += 0.3f * dt;
		}
		if (window->GetKey(GLFW_KEY_A))
		{
			phi += 0.3f * dt;
		}
		if (window->GetKey(GLFW_KEY_S))
		{
			theta -= 0.3f * dt;
		}
		if (window->GetKey(GLFW_KEY_D))
		{
			phi -= 0.3f * dt;
		}
		eye = glm::vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta)) + center;
	}
}

int main()
{
	window = std::unique_ptr<Window>(new Window("CSCI3090 - Assignment 4", WIDTH, HEIGHT));

	PTrace ptrace("ptrace20.txt", "simple.vertshader", "simple.fragshader");
	center = ptrace.GetCenter();
	theta = 1.5f;
	phi = 1.5f;
	r = 2000.0f;
	eye = glm::vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta)) + center;

	glLineWidth(1.2f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	// variables used to calculate delta time
	clock_t start, end;
	float dt = 0;

	while (!window->Closed())
	{
		start = clock();

		window->Clear();
		update(dt);
		recalc_matrices(dt);
#if ASSIGNMENT_PART
		ptrace.DrawTubes(view_persp, model);
#else
		ptrace.DrawLines(view_persp, model);
#endif
		window->Update();

		end = clock();
		dt = float(end - start) / CLOCKS_PER_SEC;
	}

	return 0;
}