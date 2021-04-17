#pragma once

#include <vector>

#include <glad/glad.h>

#include "Vertex.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray(const VertexBuffer &vertexBuffer);

	GLuint getVAO() const;

	size_t getNumberOfIndicies();
	size_t getNumberOfVertices();

private:
	void initialize(const VertexBuffer& vertexBuffer);

	GLuint VAO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

