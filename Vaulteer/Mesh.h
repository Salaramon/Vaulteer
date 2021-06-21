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
	void draw();
	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
	VertexAttribute vertexAttribute;
};

