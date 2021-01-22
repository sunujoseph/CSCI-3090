/************************************************
 *
 *             Example Four
 *
 *  A basic OpenGL program that draws a
 *  triangle on the screen in perspective with
 *  simple control over the eye position.  
 *  This program illustrates the construction of
 *  perspective and viewing transformations.
 *
 ************************************************/

#include <Windows.h>
#include <gl/glew.h>
#include <glm/ext.hpp>
#define GLFW_DLL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;


GLuint program;			// shader programs
//GLuint triangleVAO;		// the data to be displayed
GLuint objVAO;   // VAO for line
GLuint pointVAO; // VAO for points
float angle = 0.0;
double theta, phi;		// user's position  on a sphere centered on the object
double r;				// radius of the sphere
GLuint ibuffer;

float cx, cy, cz;

glm::mat4 projection;	// projection matrix
float eyex, eyey, eyez;	// eye position
vector<int> n_lines;
vector<int> n_points;
float TENSION = 0;
bool first = true;
GLfloat* vertices;
GLushort* indices;

/*
 *  The init procedure creates the OpenGL data structures
 *  that contain the triangle geometry, compiles our
 *  shader program and links the shader programs to
 *  the data.
 */

void init() {
	// allow calling init() again to change tension
	if (first)
	{
		glGenVertexArrays(1, &objVAO);
		glGenVertexArrays(1, &pointVAO);
		first = false;
	}
	glBindVertexArray(objVAO);

	int num_samples = 10;
	ifstream pi("ptrace20.txt"); //ptrace20.txt
	vector<vector<glm::vec3>> totalSamples;
	vector<vector<glm::vec3>> totalPoints;
	int np = 0;
	int numPoints = 0;
	int count;
	float xmin, ymin, zmin;
	float xmax, ymax, zmax;
	int idk, idk2;
	pi >> idk >> idk2;
	pi >> xmin >> xmax;
	pi >> ymin >> ymax;
	pi >> zmin >> zmax;
	for (int k = 0; k < 504; k++) 
	{
		vector<glm::vec3> points;
		pi >> count;
		n_lines.push_back(count);
		cx = (xmin + xmax) / 2.0f;
		cy = (ymin + ymax) / 2.0f;
		cz = (zmin + zmax) / 2.0f;
		// read in the points
		for (int i = 0; i < count; i++)
		{
			float x, y, z;
			pi >> x >> y >> z;
			//cout << x << " " << y << " " << z << endl;
			points.push_back(glm::vec3(x, y, z));
			numPoints++;
		}
		totalPoints.push_back(points);
		vector<glm::vec3> samples;
		for (int i = 0; i < points.size(); ++i)
		{
			samples.push_back(points[i]);
			np++;
		}
		totalSamples.push_back(samples);
	}

	GLuint vbuffer;
	
	GLint vPosition;
	GLint vNormal;
	int vs;
	int fs;
	int offset = 0;
	vertices = new GLfloat[np * 3];
	indices = new GLushort[np];
	int o = 0;
	int q = 0;
	for (int k = 0; k < totalPoints.size(); k++)
	{
		//int np = totalSamples[k].size();
		int npp = n_lines[k];
		//cout << k << endl;


		for (int i = 0; i < npp; i++)
		{
			vertices[o++] = totalSamples[k][i].x;
			//cout << totalSamples[k][i].x << endl;
			vertices[o++] = totalSamples[k][i].y;
			vertices[o++] = totalSamples[k][i].z;
		}

		//n_lines.push_back(npp);
		for (int i = 0; i < npp; i++)
		{
			indices[q] = q;
			++q;
		}
		//offset += npp;
	}


	/*
		*  load the vertex coordinate data
		*/
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, np * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);



	/*
		*  load the vertex indexes
		*/
	glGenBuffers(1, &ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, np * sizeof(GLushort), indices, GL_STATIC_DRAW);

	/*
		*  compile and build the shader program
		*/
	vs = buildShader(GL_VERTEX_SHADER, "a1q2.vs");
	fs = buildShader(GL_FRAGMENT_SHADER, "a1q2.fs");
	program = buildProgram(vs, fs, 0);

	/*
		*  link the vertex coordinates to the vPosition
		*  variable in the vertex program.  Do the same
		*  for the normal vectors.
		*/
	glUseProgram(program);
	vPosition = glGetAttribLocation(program, "vPosition");
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);



	glEnableVertexAttribArray(vPosition);

}

void framebufferSizeCallback(GLFWwindow *window, int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).

	if (h == 0)
		h = 1;

	float ratio = 1.0f * w / h;

	glfwMakeContextCurrent(window);

	glViewport(0, 0, w, h);

	//projection = glm::perspective(0.7f, ratio, 1.0f, 100.0f);
	projection = glm::perspective(45.0f, ratio, 0.1f, 10000.0f);

}

/*
 *  This procedure is called each time the screen needs
 *  to be redisplayed
 */
void display() {
	
	glLineWidth(2);
	glPointSize(5);
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 modelViewPerspective;
	int modelLoc;
	int normalLoc;
	int countLines = 0;

	model = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(view * model)));

	modelViewPerspective = projection * view;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(modelViewPerspective));
	normalLoc = glGetUniformLocation(program, "normalMat");
	glUniformMatrix3fv(normalLoc, 1, 0, glm::value_ptr(normal));

	// color based on tension
	int colourLoc = glGetUniformLocation(program, "color");
	//glUniform3f(colorLoc, 1.0, 0.6 - (0.6 * TENSION), 0.0);
	//green
	glUniform4f(colourLoc, 0.0, 1.0, 0.0, 1.0);

	// draw line
	//for loop null= offset, n_lines = lines in that one
	glBindVertexArray(objVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(model));
	
	for (int i = 0; i < n_lines.size(); i++)
	{

		glDrawElements(GL_LINE_STRIP, n_lines[i], GL_UNSIGNED_SHORT, (GLvoid*)(countLines * sizeof(GLushort)));
		countLines += n_lines[i];
		//cout << n_lines[i] << "hello" << endl;
	}

	// yellow points
	glUniform3f(colourLoc, 1.0, 1.0, 0.0);

	// draw points
	for (int i = 0; i < n_points.size(); i++)
	{
		glBindVertexArray(pointVAO);
		glDrawElements(GL_POINTS, n_points[i], GL_UNSIGNED_SHORT, (GLvoid*)countLines);
		countLines += n_points[i];
		cout  << "world" << endl;
	}
	


	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);

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

	eyex = (float)(r*sin(theta)*cos(phi));
	eyey = (float)(r*sin(theta)*sin(phi));
	eyez = (float)(r*cos(theta));

	//eyex = r * sin(theta) * cos(phi) + cx;
	//eyez = 1500;
	//eyey = r * cos(theta) + cy;

}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}


int main(int argc, char **argv) {
	GLFWwindow *window;

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
	if(error != GLEW_OK) {
		printf("Error starting GLEW: %s\n",glewGetErrorString(error));
		exit(0);
	}

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POINT_SMOOTH);

	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glViewport(0, 0, 512, 512);

	//projection = glm::perspective(0.7f, 1.0f, 1.0f, 100.0f);

	init();
	init();

	//eyex = 0.0;
	//eyez = 4.0;
	//eyey = 0.0;

	//theta = 0.0;
	//phi = -1.5;
	//r = 4.0;




	theta = 1.5;
	phi = 1.5;
	r = 200;
	eyex = r * sin(theta) * cos(phi);
	eyez = r * sin(theta) * sin(phi);
	eyey = r * cos(theta);



	glfwSwapInterval(1);

	// GLFW main loop, display model, swapbuffer and check for input

	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

}