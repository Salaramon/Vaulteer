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
	Mesh(Vertices vertices, Indices indices, Material material) :
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

	void bind() {
		vertexBuffer.bindVertexArray();
		indexBuffer.bindVertexArray(VertexBuffer<Vertex>::getVAO());

		vertexBuffer.getVAO().bind();
		LOG::CLAS::debug<&Mesh::bind>(this, "Mesh was bound.");
		
	}

	void unbind() {
		vertexBuffer.getVAO().unbind();
		LOG::CLAS::debug<&Mesh::unbind>(this, "Mesh was unbound.");
	}

	void makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer) {
		LOG::CLAS::debug<&Mesh::makeInstanced>(this, "Not implemented, does nothing.");
		//relocated from constructor to here. 
		//Should locInfo an divisors be defined in the shader which it belongs to 
		//and a static function in mesh be used to set what will be used for instancing and other settings if any?
		//instanceBuffer.bindVertexArray(vertexArray, locDivisors, locDivisors);
	}

	void updateBuffer() {
		vertexBuffer.insert(vertices);
		LOG::CLAS::debug<&Mesh::updateBuffer>(this, &vertices, "Mesh's vertex buffer was updated.");
	}

	Vertices vertices;
	Indices indices;

	VertexBuffer<Vertex> vertexBuffer;
	ElementBuffer indexBuffer;

	Material material;


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

