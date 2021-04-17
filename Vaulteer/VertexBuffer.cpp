#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	initialize();
}

VertexBuffer::~VertexBuffer()
{
	cleanup();
}

GLuint VertexBuffer::getVBO() const
{
	return VBO;
}

GLuint VertexBuffer::getEBO() const
{
	return EBO;
}

void VertexBuffer::initialize() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void VertexBuffer::cleanup() 
{
	//glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}