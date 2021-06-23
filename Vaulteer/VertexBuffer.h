#pragma once

#include <vector>

#include<glad/glad.h>

#include "GLSLCPPBinder.h"

#include "Vertex.h"

typedef std::vector<Vertex> Vertices;
typedef GLuint Index;
typedef std::vector<GLuint> Indices;

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(VertexBuffer&& other) :
		VBO(other.VBO),
		EBO(other.EBO)
	{
		other.VBO = 0;
		other.EBO = 0;
	}
	~VertexBuffer();

	GLuint getVBO() const;
	GLuint getEBO() const;

private:
	void initialize();
	void cleanup();

	GLuint VBO;
	GLuint EBO;

};

