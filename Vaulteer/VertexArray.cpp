#include "VertexArray.h"

VertexArray::VertexArray(const VertexBuffer& vertexBuffer)
{
	initialize(vertexBuffer);
}

void VertexArray::initialize(const VertexBuffer& vertexBuffer)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getVBO());
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer.getEBO());
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(Vertex), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLuint VertexArray::getVAO() const
{
	return VAO;
}

size_t VertexArray::getNumberOfIndicies()
{
	return indices.size();
}

size_t VertexArray::getNumberOfVertices()
{
	return vertices.size();
}