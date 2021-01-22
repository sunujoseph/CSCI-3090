// Marc Tremblay
// 100555250
// Apr 7, 2016

#ifndef PTRACE_H
#define PTRACE_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "GL/glew.h"

struct Header
{
	// store the header values
	int max_paths, max_path_length;
	float min_x, max_x;
	float min_y, max_y;
	float min_z, max_z;
};

class PTrace
{
private:
	// store the ptrace filename
	std::string filename;
	Header header;
	GLuint shader_prog;

	// store all the points from all paths
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> points_tube;
	std::vector<GLushort> indices;
	std::vector<GLushort> indices_tube;
	GLuint vao;
	GLuint vao_tube;

	// read in the values from the file and generate line indices
	void LoadPTrace();

	// compute the vertices and indices for the tube representation
	void ComputeTubes();

	// generate the buffers for the line and tube representations
	void GenBuffers();

public:
	PTrace(std::string filename, const char *vertex_file, const char *fragment_file);
	void DrawLines(glm::mat4 vp, glm::mat4 mod);
	void DrawTubes(glm::mat4 vp, glm::mat4 mod);

	// get the center of the paths based on header data
	inline glm::vec3 GetCenter() { return glm::vec3((header.max_x + header.min_x)/2, (header.max_y + header.min_y)/2, (header.max_z + header.min_z)/2); }
};

#endif // PTRACE_H