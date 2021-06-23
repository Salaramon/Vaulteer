#pragma once

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexAttribute.h"

#include "Shader.h"

class Mesh
{
public:
	Mesh(Vertices vertices, Indices indices) :
		vertexArray(vertexBuffer, vertices, indices),
		vertexAttribute(vertexArray) 
	{}
	Mesh(Mesh&& other) : 
		vertexBuffer(std::move(other.vertexBuffer)),
		vertexArray(std::move(other.vertexArray)),
		vertexAttribute(std::move(other.vertexAttribute))
	{}
	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
	VertexAttribute vertexAttribute;
};

