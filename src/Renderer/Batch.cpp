#include "vpch.h"
#include "Renderer/Batch.h"


Batch::Batch(GLint textureID, size_t vertexBufferSize, size_t indexBufferSize) :
		textureID(textureID), vertexBufferSize(vertexBufferSize), indexBufferSize(indexBufferSize) {

	vertexBuffer = vertexArray.createVertexBuffer(MaterialVertex::getFormat());
	vertexBuffer->reserve(vertexBufferSize);
	indexBuffer = vertexArray.createIndexBuffer();
	indexBuffer->reserve(indexBufferSize);

	std::cout << "Batch created with size " << vertexBufferSize << "/" << indexBufferSize << "." << std::endl;
}

Batch::~Batch() {
	std::cout << "Batch destroyed." << std::endl;
}

bool Batch::add(Mesh& mesh, glm::mat4 modelMat) {
	if (vertexBufferSize - numVertices < mesh.vertexContainer.size() || 
		indexBufferSize - numIndices < mesh.indices.size()) {
		return false;
	}

	std::vector<MaterialVertex> vertices = mesh.getCopiedData<MaterialVertex>();
	for (auto& vertex : vertices) {
		// transform from mesh space to batch space
		glm::vec4 pos = (modelMat * glm::vec4(vertex.aPos, 1.0));
		vertex.aPos = glm::vec3(pos.x, pos.y, pos.z);
		glm::vec4 norm = (glm::inverse(glm::transpose(modelMat)) * glm::vec4(vertex.aNormal, 1.0));
		vertex.aNormal = glm::normalize(glm::vec3(norm.x, norm.y, norm.z));
	}

	std::vector<GLuint> indices = mesh.indices;
	for (int i = 0; i < indices.size(); i++) {
		indices[i] += numVertices;
	}

	vertexBuffer->insertPartial(numVertices, vertices);
	numVertices += vertices.size();

	indexBuffer->insertPartial(numIndices, indices);
	numIndices += indices.size();

	return true;
}

void Batch::clear() {
	vertexBuffer->reserve(numVertices);
	indexBuffer->reserve(numIndices);
	numVertices = 0;
	numIndices = 0;
}

void Batch::bind() {
	vertexArray.bind();
}

void Batch::unbind() {
	vertexArray.unbind();
}
