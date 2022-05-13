#pragma once

#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "Mesh.h"

class Batch {
public:
	Batch(VertexArray& vertexArray, GLint textureID, size_t vertexBufferSize, size_t indexBufferSize);
	~Batch();

	bool add(const Mesh& mesh, glm::mat4 modelMat);

	void clear();

	size_t numVertices = 0;
	size_t numIndices = 0;
	GLint textureID;

private:
	VertexArray& vertexArray;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;

	size_t vertexBufferSize;
	size_t indexBufferSize;
};

