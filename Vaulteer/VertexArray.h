#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray(const VertexBuffer &vertexBuffer, const Vertices& vertices, const Indices& indices);
	~VertexArray();
	GLuint getVAO() const;

	void bind();
	void unbind();

	Vertices vertices;
	Indices indices;

private:
	void initialize(const VertexBuffer& vertexBuffer);
	void cleanup();

	GLuint VAO;

	inline static VertexArray* boundArray = nullptr;
};

