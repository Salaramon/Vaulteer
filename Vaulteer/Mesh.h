#pragma once

#include <vector>

#include <glad/glad.h>

#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);

private:
	//VertexBuffer vertexBuffer;
	//VertexArray vertexArray;
};

