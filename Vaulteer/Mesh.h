#pragma once

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"

#include "Material.h"
#include "Shader.h"

#include "DebugLogger.h"

class Mesh : public DebugLogger<Mesh> {
public:
	Mesh(Vertices vertices, Indices indices) :
		vertices(vertices),
		indices(indices),
		material({}),
		vertexBuffer(vertices, vertexArray, Vertex::locInfo),
		indexBuffer(indices, vertexArray) {
		debug("Mesh created.\n");
	}
	Mesh(Vertices vertices, Indices indices, Material material) :
		vertices(vertices),
		indices(indices),
		material(material),
		vertexBuffer(vertices, vertexArray, Vertex::locInfo),
		indexBuffer(indices, vertexArray) {
		debug("Mesh created.\n");
	}
	Mesh(const Mesh& other) noexcept :
		vertices(other.vertices),
		indices(other.indices),
		material(other.material),
		vertexBuffer(vertices, vertexArray, Vertex::locInfo),
		indexBuffer(indices, vertexArray) {
		debug("Mesh copied.\n");
	}
	Mesh(Mesh&& other) noexcept : 
		vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		material(std::move(other.material)),
		vertexArray(std::move(other.vertexArray)),
		vertexBuffer(std::move(other.vertexBuffer)),
		indexBuffer(std::move(other.indexBuffer)) {
		debug("Mesh moved.\n");
	}
	~Mesh() {
		debug("Mesh destroyed.\n");
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

	VertexArray vertexArray;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;

	Material material;
};

