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
	Mesh(Vertices vertices, Indices indices) :
		vertices(vertices),
		indices(indices),
		vertexBuffer(vertices, vertexArray, locInfo),
		indexBuffer(indices, vertexArray)
	{
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

	void makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer) {
		//relocated from constructor to here. 
		//Should locInfo an divisors be defined in the shader which it belongs to 
		//and a static function in mesh be used to set what will be used for instancing and other settings if any?
		instanceBuffer.bindVertexArray(vertexArray, locDivisors, locDivisors);
	}

	
	inline static LocationVector locInfo = {
		Binder::geometry_vertex::locations::aPos,
		Binder::geometry_vertex::locations::aNormal,
		Binder::geometry_vertex::locations::aTexCoords
	};

	inline static LocationVector locDivisors = {
		//Binder::geometry_vertex::locations::instanceMatrix
	};

	Vertices vertices;
	Indices indices;

	VertexArray vertexArray;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;
};

