#include "vpch.h"
#include "Model/Mesh.h"


void Mesh::bind() {
	vertexBuffer.bindVertexArray();
	indexBuffer.bindVertexArray(VertexBuffer<Vertex>::getVAO());

	VertexBuffer<Vertex>::getVAO().bind();
	LOG::CLAS::debug<&Mesh::bind>(this, "Mesh was bound.");
}

void Mesh::unbind() {
	VertexBuffer<Vertex>::getVAO().unbind();
	LOG::CLAS::debug<&Mesh::unbind>(this, "Mesh was unbound.");
}

void Mesh::makeInstanced(VertexBuffer<glm::mat4>& instanceBuffer) {
	LOG::CLAS::debug<&Mesh::makeInstanced>(this, "Not implemented, does nothing.");
	//relocated from constructor to here. 
	//Should locInfo an divisors be defined in the shader which it belongs to 
	//and a static function in mesh be used to set what will be used for instancing and other settings if any?
	//instanceBuffer.bindVertexArray(vertexArray, locDivisors, locDivisors);
}

void Mesh::updateBuffer() {
	vertexBuffer.insert(vertices);
	LOG::CLAS::debug<&Mesh::updateBuffer>(this, &vertices, "Mesh's vertex buffer was updated.");
}
