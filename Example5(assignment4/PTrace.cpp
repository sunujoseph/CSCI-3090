// Marc Tremblay
// 100555250
// Apr 7, 2016

#include "PTrace.h"
#include "Shaders.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

PTrace::PTrace(std::string filename, const char *vertex_file, const char *fragment_file)
	: filename(filename)
{
	char *vf = const_cast<char*>(vertex_file);
	char* ff = const_cast<char*>(fragment_file);
	int vs = buildShader(GL_VERTEX_SHADER, vf);
	int fs = buildShader(GL_FRAGMENT_SHADER, ff);
	shader_prog = buildProgram(vs, fs);
	dumpProgram(shader_prog, "Assignment 4");

	LoadPTrace();
	ComputeTubes();
	GenBuffers();
}

void PTrace::LoadPTrace()
{
	std::ifstream pi(filename, std::ifstream::in);
	
	pi >> header.max_paths;
	pi >> header.max_path_length;
	pi >> header.min_x;
	pi >> header.max_x;
	pi >> header.min_y;
	pi >> header.max_y;
	pi >> header.min_z;
	pi >> header.max_z;

	std::cout << "Header" << std::endl;
	std::cout << header.max_paths << ", " << header.max_path_length << std::endl;
	std::cout << header.min_x << ", " << header.max_x << std::endl;
	std::cout << header.min_y << ", " << header.max_y << std::endl;
	std::cout << header.min_z << ", " << header.max_z << std::endl;

	int p_points;
	int start = 0;

	while (pi >> p_points)
	{
		float x, y, z;
		pi >> x;
		pi >> y;
		pi >> z;
		points.push_back(glm::vec3(x, y, z));

		for (int i = 1; i < p_points; i++)
		{
			// get point data
			pi >> x;
			pi >> y;
			pi >> z;
			points.push_back(glm::vec3(x, y, z));

			//generate indices
			indices.push_back(start + i - 1);
			indices.push_back(start + i);
		}
		start += p_points;
	}
}

void PTrace::ComputeTubes()
{
	for (int i = 0; i < points.size() - 1; i++)
	{
		float x;
		float y;
		float z;
		if (i < (points.size() - 1))
		{
			// p_next - p_curr
			x = points[i + 1].x - points[i].x;
			y = points[i + 1].y - points[i].y;
			z = points[i + 1].z - points[i].z;
		}
		else if (i == (points.size() - 1))
		{
			// p_curr - p_prev
			x = points[i].x - points[i - 1].x;
			y = points[i].y - points[i - 1].y;
			z = points[i].z - points[i - 1].z;
		}

		glm::vec3 a(x, y, z);
		glm::vec3 b;

		// the vectors used to compute new verts
		glm::vec3 u, v;

		if (x >= y) {
			if (y >= z) b = glm::vec3(y, -x, z);
			else b = glm::vec3(z, y, -x);
		}
		else {
			if (x >= z) b = glm::vec3(y, -x, z);
			else b = glm::vec3(x, z, -y);
		}
		u = glm::cross(a, b);
		u = glm::normalize(u);
		v = glm::cross(a, u);
		v = glm::normalize(v);


		// compute new vertices
		glm::vec3 p1, p2, p3, p4; //points
		// calculate the vertices and push them to the list
		p1 = points[i] + u * 4.0f; points_tube.push_back(p1);
		p2 = points[i] - u * 4.0f; points_tube.push_back(p2);
		p3 = points[i] + v * 4.0f; points_tube.push_back(p3);
		p4 = points[i] - v * 4.0f; points_tube.push_back(p4);

		if (i > 0)
		{
			/*
			n = point on the line
				pc1 = n*4 + 0
				pc2 = n*4 + 1
				pc3 = n*4 + 2
				pc4 = n*4 + 3
			previous indices
				pp1 = [n-1]*4 + 0
				pp2 = [n-1]*4 + 1
				pp3 = [n-1]*4 + 2
				pp4 = [n-1]*4 + 3
			*/
			int pc1 = i * 4 + 0;
			int pc2 = i * 4 + 1;
			int pc3 = i * 4 + 2;
			int pc4 = i * 4 + 3;

			int pp1 = (i - 1) * 4 + 0;
			int pp2 = (i - 1) * 4 + 1;
			int pp3 = (i - 1) * 4 + 2;
			int pp4 = (i - 1) * 4 + 3;

			indices_tube.push_back(pp1); indices_tube.push_back(pc1); indices_tube.push_back(pp2);
			indices_tube.push_back(pp2); indices_tube.push_back(pc1); indices_tube.push_back(pc2);
			indices_tube.push_back(pc2); indices_tube.push_back(pc3); indices_tube.push_back(pp2);
			indices_tube.push_back(pp2); indices_tube.push_back(pc3); indices_tube.push_back(pp3);

			indices_tube.push_back(pp3); indices_tube.push_back(pc3); indices_tube.push_back(pp4);
			indices_tube.push_back(pp4); indices_tube.push_back(pc3); indices_tube.push_back(pc4);
			indices_tube.push_back(pc4); indices_tube.push_back(pc1); indices_tube.push_back(pp4);
			indices_tube.push_back(pp4); indices_tube.push_back(pc1); indices_tube.push_back(pp1);
		}
	}
}

// add in the buffer generation for the tubes
void PTrace::GenBuffers()
{
	//lines
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vBuff;
	glGenBuffers(1, &vBuff);
	glBindBuffer(GL_ARRAY_BUFFER, vBuff);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
	
	GLuint iBuff;
	glGenBuffers(1, &iBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
	
	GLint pos_attr = glGetAttribLocation(shader_prog, "position");
	glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_attr);


	//tubes
	glGenVertexArrays(1, &vao_tube);
	glBindVertexArray(vao_tube);

	GLuint vBuffT;
	glGenBuffers(1, &vBuffT);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffT);
	glBufferData(GL_ARRAY_BUFFER, points_tube.size() * sizeof(glm::vec3), &points_tube[0], GL_STATIC_DRAW);

	GLuint iBuffT;
	glGenBuffers(1, &iBuffT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_tube.size() * sizeof(GLushort), &indices_tube[0], GL_STATIC_DRAW);

	pos_attr = glGetAttribLocation(shader_prog, "position");
	glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pos_attr);

}

void PTrace::DrawLines(glm::mat4 vp, glm::mat4 mod)
{
	glUseProgram(shader_prog);

	GLint vp_loc = glGetUniformLocation(shader_prog, "vp_mat");
	glUniformMatrix4fv(vp_loc, 1, 0, glm::value_ptr(vp));
	GLint mod_loc = glGetUniformLocation(shader_prog, "mod_mat");
	glUniformMatrix4fv(mod_loc, 1, 0, glm::value_ptr(mod));
	
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, points.size(), GL_UNSIGNED_SHORT, NULL);
}

void PTrace::DrawTubes(glm::mat4 vp, glm::mat4 mod)
{
	glUseProgram(shader_prog);

	GLint vp_loc = glGetUniformLocation(shader_prog, "vp_mat");
	glUniformMatrix4fv(vp_loc, 1, 0, glm::value_ptr(vp));
	GLint mod_loc = glGetUniformLocation(shader_prog, "mod_mat");
	glUniformMatrix4fv(mod_loc, 1, 0, glm::value_ptr(mod));

	glBindVertexArray(vao_tube);
	glDrawElements(GL_TRIANGLES, indices_tube.size() / 3, GL_UNSIGNED_SHORT, NULL);
}