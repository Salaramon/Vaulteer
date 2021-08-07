#pragma once

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"


#include "Shader.h"

#include "DebugLogger.h"

class Mesh : public DebugLogger<Mesh>
{
public:
	Mesh(Vertices vertices, Indices indices, VertexBuffer<glm::mat4>& instanceBuffer) :
		vertices(vertices),
		indices(indices),
		vertexBuffer(vertices, vertexArray, locInfo),
		indexBuffer(indices, vertexArray)
	{
		instanceBuffer.bindVertexArray(vertexArray, locDivisors, locDivisors);
		debug("Mesh created.\n");
	}
	Mesh(Mesh&& other) noexcept : 
		vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		vertexArray(std::move(other.vertexArray)),
		vertexBuffer(std::move(other.vertexBuffer)),
		indexBuffer(std::move(other.indexBuffer))
	{
		debug("Mesh moved.\n");
	}
	~Mesh() {
		debug("Mesh destroyed.\n");
	}

	
	inline static LocationVector locInfo = {
		Binder::vertex::locations::aPos,
		Binder::vertex::locations::aNormal,
		Binder::vertex::locations::aTexCoords
	};

	inline static LocationVector locDivisors = {
		Binder::vertex::locations::instanceMatrix
	};

	Vertices vertices;
	Indices indices;

	VertexArray vertexArray;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;
};

