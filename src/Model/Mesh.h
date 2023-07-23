#pragma once

#include <glad/glad.h>
#include <map>

#include "Model/VertexBuffer.h"
#include "Model/IndexBuffer.h"
#include "Model/VertexArray.h"

#include "Model/Material.h"
#include "Storage/Geometry.h"
#include "Storage/VertexImpl.h"


struct Mesh {
	Geometry* geometry;
	Material* material;

	std::vector<glm::mat4> instanceMats;
	int instanceCount;

	VertexArray vertexArray;
	std::vector<VertexBuffer*> vertexBuffers;
	IndexBuffer* indexBuffer;

	Mesh(Geometry* geometry, Material* material) :
		geometry(geometry),
		material(material) {

		initialize();
		
		instanceCount = 0;
		glm::mat4 model(1.0);
		vertexBuffers[2]->insert(&model, 1);
	}

	Mesh(Geometry* geometry, Material* material, const glm::mat4& instanceMat) :
		geometry(geometry),
		material(material) {

		initialize();
		
		instanceCount = 1;
		vertexBuffers[2]->insert(&instanceMat, 1);
	}

	Mesh(const Mesh& other) :
		geometry(other.geometry),
		material(other.material) {
		// creates a new VAO with blank instance data for the copied mesh
		initialize();

		instanceCount = 1;
		glm::mat4 model(1.0);
		vertexBuffers[2]->insert(&model, 1);
	}

	Mesh(Mesh&& other) noexcept :
		geometry(other.geometry),
		material(other.material),
		vertexArray(std::move(other.vertexArray)),
		vertexBuffers(other.vertexBuffers),
		indexBuffer(other.indexBuffer) {}

	~Mesh() = default;

	void initialize() {
		vertexBuffers = vertexArray.createVertexBuffers(geometry->vertices.getFormat());
		indexBuffer = vertexArray.createIndexBuffer(geometry->indices);

		insertVertices();
		insertMaterial();
	}

	void bind() {
		vertexArray.bind();
	}

	static void unbind() {
		VertexArray::unbind();
	}

	void overrideMaterial(Material* material) {
		this->material = material;
		insertMaterial();
	}


	void insertVertices() {
		vertexBuffers[0]->insert(geometry->vertices.data(), geometry->vertices.size());
	}

	void insertMaterial() {
		int matIndex = material->materialIndex;
		vertexBuffers[1]->insert(&matIndex, 1, GL_DYNAMIC_DRAW);
	}


	void pushInstance(const glm::mat4& instance) {
		instanceMats.push_back(instance);
		instanceCount++;
		insertInstances();
	}

	void updateInstances(const std::vector<glm::mat4>& instances) {
		instanceMats = instances;
		instanceCount = instances.size();
		insertInstances();
	}

	void updateInstance(const glm::mat4& instance, size_t index) {
		KYSE_ASSERT(index < instanceMats.size())
		instanceMats[index] = instance;
		insertInstance(index);
	}
	
	void insertInstances() {
		KYSE_ASSERT(!instanceMats.empty() && instanceCount > 0)
		vertexBuffers[2]->insert(instanceMats.data(), instanceCount);
	}

	void insertInstance(size_t index) {
		KYSE_ASSERT(index < instanceMats.size())
		glm::mat4* instance = &instanceMats[index];
		vertexBuffers[2]->insertPartial(index, instance, 1);
	}


	bool hasMultipleInstances() {
		return instanceCount > 1;
	}

	GLenum type() {
		return geometry->type;
	}

	GLsizei numVertices() {
		return (GLsizei) geometry->vertices.size();
	}

	GLsizei numIndices() {
		return (GLsizei) geometry->indices.size();
	}

};
