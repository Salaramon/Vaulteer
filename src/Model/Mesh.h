#pragma once

#include <glad/glad.h>

#include "Model/VertexBuffer.h"
#include "Model/ElementBuffer.h"
#include "Model/VertexArray.h"

#include "Model/Material.h"
#include "Renderer/Shader.h"

#include "Debug/DebugLogger.h"

class Mesh : public DebugLogger<Mesh> {
public:
	Mesh(Vertices vertices, Indices indices) :
		vertices(vertices),
		indices(indices),
		vertexBuffer(vertices),
		indexBuffer(indices, vertexBuffer.getVAO()) {
		debug("Mesh created.\n");
	}
	Mesh(Vertices vertices, Indices indices, Material material) :
		vertices(vertices),
		indices(indices),
		material(material),
		vertexBuffer(vertices),
		indexBuffer(indices, vertexBuffer.getVAO()) {
		debug("Mesh created.\n");
	}
	Mesh(const Mesh& other) noexcept :
		vertices(other.vertices),
		indices(other.indices),
		material(other.material),
		vertexBuffer(vertices),
		indexBuffer(indices, vertexBuffer.getVAO()) {
		debug("Mesh copied.\n");
	}
	Mesh(Mesh&& other) noexcept : 
		vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		material(std::move(other.material)),
		vertexBuffer(std::move(other.vertexBuffer)),
		indexBuffer(std::move(other.indexBuffer)) {
		debug("Mesh moved.\n");
	}
	~Mesh() {
		debug("Mesh destroyed.\n");
	}

	void bind() {
		vertexBuffer.bindVertexArray();
		indexBuffer.bindVertexArray(VertexBuffer<Vertex>::getVAO());

		vertexBuffer.getVAO().bind();
	}

	void unbind() {
		vertexBuffer.getVAO().unbind();
	}

	void makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer) {
		//relocated from constructor to here. 
		//Should locInfo an divisors be defined in the shader which it belongs to 
		//and a static function in mesh be used to set what will be used for instancing and other settings if any?
		//instanceBuffer.bindVertexArray(vertexArray, locDivisors, locDivisors);
	}

	void updateBuffer() {
		vertexBuffer.insert(vertices);
	}

	Vertices vertices;
	Indices indices;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;

	Material material;
};

