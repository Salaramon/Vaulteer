#pragma once

#include "Model/VertexBuffer.h"
#include "Model/IndexBuffer.h"
#include "Model/Storage/MaterialVertex.h"

#include "Model/Mesh.h"

class Batch {
public:
	Batch(GLint textureID, size_t vertexBufferSize, size_t indexBufferSize);
	~Batch();

	bool add(Mesh& mesh, glm::mat4 modelMat);

	void clear();

	size_t numVertices = 0;
	size_t numIndices = 0;
	GLint textureID;

	void bind();
	void unbind();

	VertexArray vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

private:
	size_t vertexBufferSize;
	size_t indexBufferSize;
};

