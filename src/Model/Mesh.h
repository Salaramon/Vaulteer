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
	Mesh(Vertices vertices, Indices indices);
	Mesh(Vertices vertices, Indices indices, std::shared_ptr<Material> material);
	Mesh(const Mesh& other) noexcept;
	Mesh(Mesh&& other) noexcept;

	~Mesh();

	void bind();
	void unbind();

	void makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer);
	void updateBuffer();

	Vertices vertices;
	Indices indices;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;

	std::shared_ptr<Material> material;
};

