/************************************************
 *
 *             Example Five
 *
 *  The stickman program.  This example shows
 *  how a hierarchical model can be constructed
 *  and displayed using OpenGL
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
#include <stack>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

GLuint program;			// shader programs

glm::mat4 projection;	// projection matrix
float eyex, eyey, eyez;	// eye position
double theta, phi;		// user's position  on a sphere centered on the object
double r;				// radius of the sphere
float maxpath, maxlen;
float cx, cy, cz;

struct Master {
	GLuint vao;
	int indices;
	GLuint ibuffer;
	GLuint vbuffer;
};

Master** stru = new Master*[504];
//Master *leg;
//Master *arm;
ifstream pi("ptrace20.txt"); //ptrace20.txt


Master* cylinder(int numPoints) { // Get the current stream and number of points
	Master* result;
	GLuint vao;
	int i;
	GLfloat* vertices;
	GLushort* indices;
	int j;
	GLuint vbuffer;
	GLuint ibuffer;
	GLint vPosition;
	result = new Master;


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	result->vao = vao;
	result->indices = numPoints;

	vertices = new GLfloat[3 * (numPoints)]; // this would be GLfloat[3*numPoints]
	j = 0;

	for (i = 0; i < numPoints; i++) { // You would use this loop read all the vertices for the current line.
								   // for(int i = 0; i < numpoints; i++) { read three points}
		pi >> vertices[j++] >> vertices[j++] >> vertices[j++];

	}
	indices = new GLushort[numPoints]; // this would be GLushort[numPoints]
										   // generate numbers from 0 to numPoints [0,1,2...103] for first one

	j = 0;
	for (int h = 0; h < numPoints; h++) {
		indices[h] = h;
	}

	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * (numPoints) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &ibuffer);
	result->ibuffer = ibuffer;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numPoints * sizeof(GLushort), indices, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	return(result);
}

/*
 *  The init procedure creates the OpenGL data structures
 *  that contain the triangle geometry, compiles our
 *  shader program and links the shader programs to
 *  the data.
 */

void init() {
	int vs;
	int fs;
	int count;
	float xmin, ymin, zmin;
	float xmax, ymax, zmax;
	
	pi >> maxpath >> maxlen;
	pi >> xmin >> xmax;
	pi >> ymin >> ymax;
	pi >> zmin >> zmax;

	cx = (xmin + ymin) / 2;
	cy = (xmin + ymin) / 2;
	cz = (xmin + ymin) / 2;

	for (int k = 0; k < 504; k++)
	{
		vector<glm::vec3> points;
		pi >> count;
		stru[k] = cylinder(count);
	}

	/*
	 *  compile and build the shader program
	 */
	vs = buildShader(GL_VERTEX_SHADER, "example5.vs");
	fs = buildShader(GL_FRAGMENT_SHADER, "example5.fs");
	program = buildProgram(vs, fs, 0);
	dumpProgram(program, "example 5");

	glUseProgram(program);


}

/*
 *  Executed each time the window is resized,
 *  usually once at the start of the program.
 */
void framebufferSizeCallback(GLFWwindow* window, int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).

	if (h == 0)
		h = 1;

	float ratio = 1.0f * w / h;

	glfwMakeContextCurrent(window);

	glViewport(0, 0, w, h);

	projection = glm::perspective(0.7f, ratio, 1.0f, 100.0f);

}

/*
 *  This procedure is called each time the screen needs
 *  to be redisplayed
 */
void display() {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 viewPerspective;
	int viewLoc;
	int modelLoc;
	stack<glm::mat4> matrixStack;
	GLint vPosition;
	int colourLoc;

	
	view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
		   glm::vec3(cx,cy,cz),
		   glm::vec3(0.0f, 0.0f, 1.0f));

	viewPerspective = projection * view;
	model = glm::mat4(1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	viewLoc = glGetUniformLocation(program, "viewPerspective");
	glUniformMatrix4fv(viewLoc, 1, 0, glm::value_ptr(viewPerspective));
	modelLoc = glGetUniformLocation(program, "model");
	colourLoc = glGetUniformLocation(program, "colour");
	vPosition = glGetAttribLocation(program, "vPosition");

	/* body colour - green */

	//glUniform4f(colourLoc, 0.0, 1.0, 0.0, 1.0);
	glUniform4f(colourLoc, 1.0, 0.6, 0.0, 1.0);

	for (int i = 0; i < 504; i++) {

		glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(model));
		glBindVertexArray(stru[i]->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stru[i]->ibuffer);
		glDrawElements(GL_LINE_STRIP, stru[i]->indices, GL_UNSIGNED_SHORT, NULL);
	}

}

/*
 *  Called each time a key is pressed on
 *  the keyboard.
 */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		phi -= 0.1;
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		phi += 0.1;
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		theta += 0.1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		theta -= 0.1;

	eyex = (float)(r * sin(theta) * cos(phi));
	eyey = (float)(r * sin(theta) * sin(phi));
	eyez = (float)(r * cos(theta));

}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}


int main(int argc, char** argv) {
	GLFWwindow* window;

	// start by setting error callback in case something goes wrong

	glfwSetErrorCallback(error_callback);

	// initialize glfw

	if (!glfwInit()) {
		fprintf(stderr, "can't initialize GLFW\n");
	}

	// create the window used by our application

	window = glfwCreateWindow(512, 512, "Example Four", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// establish framebuffer size change and input callbacks

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetKeyCallback(window, key_callback);

	/*
	 *  initialize glew
	 */
	glfwMakeContextCurrent(window);
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("Error starting GLEW: %s\n", glewGetErrorString(error));
		exit(0);
	}

	projection = glm::perspective(0.7f, 1.0f, 1.0f, 10000.0f);

	eyex = 0.0;
	eyey = 6.0;
	eyez = 0.0;

	theta = 1.3;
	phi = -1.5;
	r = 2500.0;

	init();

	//glClearColor(1.0,1.0,1.0,1.0);

	glClearColor(0.3, 0.3, 0.3, 1.0);
	//glViewport(0, 0, 512, 512);

	glfwSwapInterval(1);

	// GLFW main loop, display model, swapbuffer and check for input



	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	glfwTerminate();

}