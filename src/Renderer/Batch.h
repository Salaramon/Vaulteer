#pragma once

#include "Model/VertexBuffer.h"
#include "Model/ElementBuffer.h"

#include "Model/Mesh.h"

class Batch {
public:
	Batch(GLint textureID, size_t vertexBufferSize, size_t indexBufferSize);
	~Batch();

	bool add(const Mesh& mesh, glm::mat4 modelMat);

	void clear();

	GLint numVertices = 0;
	GLint numIndices = 0;
	GLint textureID;

	void bind();
	void unbind();

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;

private:
	size_t vertexBufferSize;
	size_t indexBufferSize;
};

