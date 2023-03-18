#pragma once

#include <glad/glad.h>

#include "Model/VertexBuffer.h"
#include "Model/ElementBuffer.h"
#include "Model/VertexArray.h"

#include "Renderer/Shader.h"


#include "Debug/Debug.h"

class Mesh {
public:
	Mesh(Vertices& vertices, Indices indices) : vertices(vertices), indices(indices) {}

	void bind() {
		vertexBuffer.bindVertexArray();
		indexBuffer.bindVertexArray(VertexBuffer<Vertex>::getVAO());

		VertexBuffer<Vertex>::getVAO().bind();
	}

	void unbind() {
		VertexBuffer<Vertex>::getVAO().unbind();
	}

	void updateBuffer() {
		vertexBuffer.insert(vertices);
	}

	Vertices vertices;
	Indices indices;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;
};

