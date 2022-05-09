#include "ElementBuffer.h"

ElementBuffer::ElementBuffer() {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
}

ElementBuffer::ElementBuffer(std::vector<Index>& indices) {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	insert(indices);
}

ElementBuffer::ElementBuffer(std::vector<Index>& indices, VertexArray& vertexArray) {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	insert(indices);
	bindVertexArray(vertexArray);
}

ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept :
	Buffer(std::move(other)) {
	debug("ElementBuffer moved. Buffer: " + std::to_string(buffer) + "\n");
}


void ElementBuffer::insert(const std::vector<Index>& indices) {
	glNamedBufferData(buffer, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);
	debug("Indices inserted into buffer: " + std::to_string(buffer) + "\n");
}

void ElementBuffer::insertPartial(size_t position, const std::vector<Index>& indices) {
	glNamedBufferSubData(buffer, position * sizeof(Index), indices.size() * sizeof(Index), indices.data());
	debug("Indices inserted into buffer: " + std::to_string(buffer) + "\n");
}

void ElementBuffer::bindVertexArray(VertexArray& vertexArray) {
	glVertexArrayElementBuffer(vertexArray, buffer);
	debug("Bound to VertexArray: " + std::to_string(vertexArray.getObjectKey()) + "\n", "glVertexArrayElementBuffer");
}
