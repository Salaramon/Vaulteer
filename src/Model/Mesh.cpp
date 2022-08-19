#include "vpch.h"
#include "Model/Mesh.h"

Mesh::Mesh(Vertices vertices, Indices indices):
	vertices(vertices),
	indices(indices),
	vertexBuffer(vertices),
	indexBuffer(indices, VertexBuffer<Vertex>::getVAO()) {
	debug("Mesh created.\n");
}

Mesh::Mesh(Vertices vertices, Indices indices, Material material):
	vertices(vertices),
	indices(indices),
	vertexBuffer(vertices),
	indexBuffer(indices, VertexBuffer<Vertex>::getVAO()),
	material(std::move(material)) {
	debug("Mesh created.\n");
}

Mesh::Mesh(const Mesh& other) noexcept:
	vertices(other.vertices),
	indices(other.indices),
	vertexBuffer(vertices),
	indexBuffer(indices, VertexBuffer<Vertex>::getVAO()),
	material(other.material) {
	debug("Mesh copied.\n");
}

Mesh::Mesh(Mesh&& other) noexcept: 
	vertices(std::move(other.vertices)),
	indices(std::move(other.indices)),
	vertexBuffer(std::move(other.vertexBuffer)),
	indexBuffer(std::move(other.indexBuffer)),
	material(std::move(other.material)) {
	debug("Mesh moved.\n");
}

Mesh::~Mesh() {
	debug("Mesh destroyed.\n");
}


void Mesh::bind() {
	vertexBuffer.bindVertexArray();
	indexBuffer.bindVertexArray(VertexBuffer<Vertex>::getVAO());

	VertexBuffer<Vertex>::getVAO().bind();
}

void Mesh::unbind() {
	VertexBuffer<Vertex>::getVAO().unbind();
}

void Mesh::makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer) {
	//relocated from constructor to here. 
	//Should locInfo an divisors be defined in the shader which it belongs to 
	//and a static function in mesh be used to set what will be used for instancing and other settings if any?
	//instanceBuffer.bindVertexArray(vertexArray, locDivisors, locDivisors);
}

void Mesh::updateBuffer() {
	vertexBuffer.insert(vertices);
}
