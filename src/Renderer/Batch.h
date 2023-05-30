#pragma once

#include "Model/VertexBuffer.h"
#include "Model/IndexBuffer.h"
#include "Model/Storage/MaterialVertex.h"

#include "Model/Mesh.h"

class Batch {
public:
	VertexArray vertexArray;
	std::vector<VertexBuffer*> vertexBuffers;
	IndexBuffer* indexBuffer;

	GLint textureID;
	
	size_t vertexBufferSize;
	size_t indexBufferSize;

	int numVertices = 0;
	int numIndices = 0;

	Batch(GLint textureID, size_t vertexBufferSize, size_t indexBufferSize) :
		textureID(textureID), vertexBufferSize(vertexBufferSize), indexBufferSize(indexBufferSize) {

		vertexBuffers = vertexArray.createVertexBuffers(MaterialVertex::getFormat());
		vertexBuffers[0]->reserve(vertexBufferSize);
		indexBuffer = vertexArray.createIndexBuffer();
		indexBuffer->reserve(indexBufferSize);

		std::cout << "Batch created with size " << vertexBufferSize << "/" << indexBufferSize << "." << std::endl;
	}

	~Batch() {
		std::cout << "Batch destroyed." << std::endl;
	}

	bool add(Mesh& mesh, glm::mat4 modelMat) {
		if (vertexBufferSize - numVertices < mesh.vertexContainer.size() || 
			indexBufferSize - numIndices < mesh.indices.size()) {
			return false;
		}

		std::vector<MaterialVertex> vertices;
		std::vector<VertexImpl> meshVertices = mesh.getCopiedData<VertexImpl>();
		vertices.reserve(meshVertices.size());

		for (int i = 0; i < mesh.vertexContainer.size(); i++) {
			auto* vertex = mesh.getVertex<VertexImpl>(i);
			// transform from mesh space to batch space
			glm::vec4 pos4 = (modelMat * glm::vec4(vertex->aPos, 1.0));
			glm::vec3 pos = glm::vec3(pos4.x, pos4.y, pos4.z);
			glm::vec4 norm4 = (glm::inverse(glm::transpose(modelMat)) * glm::vec4(vertex->aNormal, 1.0));
			glm::vec3 norm = glm::normalize(glm::vec3(norm4.x, norm4.y, norm4.z));
			vertices.emplace_back(*vertex, pos, norm, mesh.material->materialIndex);
		}

		std::vector<GLuint> indices = mesh.indices;
		for (unsigned int& index : indices) {
			index += numVertices;
		}

		vertexBuffers[0]->insertPartial(numVertices, vertices);
		numVertices += vertices.size();

		indexBuffer->insertPartial(numIndices, indices);
		numIndices += indices.size();

		return true;
	}

	void clear() {
		vertexBuffers[0]->reserve(numVertices);
		indexBuffer->reserve(numIndices);
		numVertices = 0;
		numIndices = 0;
	}

	void bind() {
		vertexArray.bind();
	}
	void unbind() {
		vertexArray.unbind();
	}
};

