#pragma once

#include <typeindex>
#include <glad/glad.h>

#include "Model/VertexBuffer.h"
#include "Model/IndexBuffer.h"
#include "Model/VertexArray.h"

#include "Model/Material.h"
#include "Storage/VertexImpl.h"


class Mesh {
public:
	template<class T>
	Mesh(std::vector<T> vertices, std::vector<GLuint>& indices, Material* material) :
		vertexContainer(vertices),
		indices(indices),
		material(material) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot create mesh of non-vertices.");

		vertexBuffers = vertexArray.createVertexBuffers(T::getFormat());
		indexBuffer = vertexArray.createIndexBuffer(this->indices);

		insertVertices();
		insertMaterial();
	}

	Mesh(Mesh& other) = delete;

	Mesh(Mesh&& other) noexcept : 
		vertexContainer(std::move(other.vertexContainer)),
		indices(std::move(other.indices)),
		material(other.material),
		vertexArray(std::move(other.vertexArray)),
		vertexBuffers(other.vertexBuffers),
		indexBuffer(other.indexBuffer) {}

	~Mesh() = default;

	void bind() {
		vertexArray.bind();
	}
	void unbind() {
		vertexArray.unbind();
	}

	template<class T>
	std::vector<T> getCopiedData() {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot copy out vertices to non-vertex type.");
		T* arr = (T*) vertexContainer.data();
		return std::vector<T>(arr, arr + vertexContainer.size());
	}
	
	template<class T>
	T* getVertex(int index) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot copy out vertices to non-vertex type.");
		return vertexContainer.at<T>(index);
	}

	void overrideMaterial(Material* material) {
		this->material = material;
		insertMaterial();
	}


	void insertVertices() {
		vertexBuffers[0]->insert(vertexContainer.data(), vertexContainer.size());
	}
	void insertMaterial() {
		vertexBuffers[1]->insert(&material->materialIndex, 1, GL_DYNAMIC_DRAW);
	}

	VertexContainer vertexContainer;
	std::vector<GLuint> indices;
	Material* material;

	VertexArray vertexArray;
	std::vector<VertexBuffer*> vertexBuffers;
	IndexBuffer* indexBuffer;
};


