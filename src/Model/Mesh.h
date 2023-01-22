#pragma once

#include <glad/glad.h>

#include "Model/VertexBuffer.h"
#include "Model/ElementBuffer.h"
#include "Model/VertexArray.h"

#include "Model/Material.h"
#include "Renderer/Shader.h"


#include "Debug/Debug.h"

class Mesh {
public:
	Mesh(Vertices vertices, Indices indices) :
		OR(this,
			DY::V(&this->vertices, &this->indices, &vertexBuffer, &indexBuffer, &material),
			DY::N("vertices", "indices", "vertexBuffer", "indexBuffer", "material")),

		vertices(vertices),
		indices(indices),
		vertexBuffer(vertices),
		indexBuffer(indices, vertexBuffer.getVAO()) {
		LOG::CTOR::debug(this, "Mesh was created");
	}
	Mesh(Vertices vertices, Indices indices, std::shared_ptr<Material> material) :
		OR(this,
			DY::V(&this->vertices, &this->indices, &vertexBuffer, &indexBuffer, &this->material),
			DY::N("vertices", "indices", "vertexBuffer", "indexBuffer", "material")),

		vertices(vertices),
		indices(indices),
		material(material),
		vertexBuffer(vertices),
		indexBuffer(indices, vertexBuffer.getVAO()) {

		LOG::CTOR::debug(this, "Mesh was created");
	}
	Mesh(const Mesh& other) noexcept :
		vertices(other.vertices),
		indices(other.indices),
		material(other.material),
		vertexBuffer(vertices),
		indexBuffer(indices, vertexBuffer.getVAO()) {

		LOG::CTOR::debug(this, "Mesh was created");
	}
	Mesh(Mesh&& other) noexcept : 
		vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		material(std::move(other.material)),
		vertexBuffer(std::move(other.vertexBuffer)),
		indexBuffer(std::move(other.indexBuffer)) {

		LOG::CTOR::debug(this, "Mesh was created");
	}
	~Mesh() {
		LOG::CTOR::debug(this, "Mesh was destroyed");
	}



	void bind();
	void unbind();

	void makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer);
	void updateBuffer();

	Vertices vertices;
	Indices indices;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;
	
	std::shared_ptr<Material> material;


	inline static auto CR = DY::ClassRegister<
		&bind,
		&unbind,
		&makeInstanced,
		&updateBuffer>();
	inline static auto CB = DY::ClassBinder<decltype(CR)>();

	DY::ObjectRegister<Mesh,
		decltype(vertices),
		decltype(indices),
		decltype(vertexBuffer),
		decltype(indexBuffer),
		decltype(material)> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};

