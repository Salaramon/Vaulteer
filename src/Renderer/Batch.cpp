#include "vpch.h"
#include "Renderer/Batch.h"

Batch::Batch(GLint textureID, size_t vertexBufferSize, size_t indexBufferSize) :
	textureID(textureID), vertexBufferSize(vertexBufferSize), indexBufferSize(indexBufferSize),
	vertexBuffer(vertexBufferSize), 
	indexBuffer(indexBufferSize, vertexBuffer.getVAO()) {
	std::cout << "Batch created with size " << vertexBufferSize << "/" << indexBufferSize << "." << std::endl;
}

Batch::~Batch() {
	std::cout << "Batch destroyed." << std::endl;
}

bool Batch::add(const Mesh& mesh, glm::mat4 modelMat) {
	if (vertexBufferSize - numVertices < mesh.vertices.size() || 
		indexBufferSize - numIndices < mesh.indices.size()) {
		return false;
	}

	Vertices vertices = mesh.vertices;
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec4 pos = (modelMat * glm::vec4(vertices[i].aPos, 1.0));
		vertices[i].aPos = glm::vec3(pos.x, pos.y, pos.z);
	}

	Indices indices = mesh.indices;
	for (int i = 0; i < indices.size(); i++) {
		indices[i] += numVertices;
	}

	vertexBuffer.insertPartial(numVertices, vertices);
	numVertices += vertices.size();

	indexBuffer.insertPartial(numIndices, indices);
	numIndices += indices.size();

	return true;
}

void Batch::clear() {
	vertexBuffer.reserve(numVertices);
	indexBuffer.reserve(numIndices);
	numVertices = 0;
	numIndices = 0;
}

void Batch::bind() {
	vertexBuffer.bindVertexArray();
	indexBuffer.bindVertexArray(VertexBuffer<Vertex>::getVAO());

	vertexBuffer.getVAO().bind();
}

void Batch::unbind() {
	vertexBuffer.getVAO().unbind();
}
