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
		
		glm::mat4 ins(1.0);
		vertexBuffers[1]->insert(&ins, 1);

		indexBuffer = vertexArray.createIndexBuffer();
		indexBuffer->reserve(indexBufferSize);

		Log::trace("Batch created with size {}/{}.", vertexBufferSize, indexBufferSize);
	}

	~Batch() {
		Log::trace("Batch destroyed.");
	}

	bool add(const Mesh& mesh, glm::mat4 modelMat) {
		Geometry* geometry = mesh.geometry;
		if (vertexBufferSize - numVertices < geometry->vertices.size() || 
			indexBufferSize - numIndices < geometry->indices.size()) {
			return false;
		}

		size_t numGeometryVertices = geometry->vertices.size();

		std::vector<MaterialVertex> vertices;
		vertices.reserve(numGeometryVertices);
		
		for (int i = 0; i < numGeometryVertices; i++) {
			// transform from mesh space to batch space
			// TODO this doesn't transform tangent/bitangent
			auto* vertex = geometry->getVertex<VertexImpl>(i);
			glm::vec4 pos4 = (modelMat * glm::vec4(vertex->aPos, 1.0));
			glm::vec3 pos = glm::vec3(pos4.x, pos4.y, pos4.z);
			glm::vec4 norm4 = (glm::inverse(glm::transpose(modelMat)) * glm::vec4(vertex->aNormal, 1.0));
			glm::vec3 norm = glm::normalize(glm::vec3(norm4.x, norm4.y, norm4.z));

			vertices.emplace_back(pos, norm, vertex->aTexCoords, vertex->aTangent, vertex->aBitangent, mesh.material->materialIndex);
		}

		std::vector<GLuint> indices = geometry->indices;
		KYSE_ASSERT(geometry->type == GL_TRIANGLES) // TODO need to handle an eventual conversion at some point...
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

	static void unbind() {
		VertexArray::unbind();
	}
};

